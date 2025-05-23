/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/23 23:00:14 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../header/minishell.h"
#include <sys/wait.h>

/**
 * @brief Finds the matching closing parenthesis for an opening one
 * 
 * @param start Token with opening parenthesis
 * @return t_list* Token with matching closing parenthesis, NULL if not found
 */
t_list	*ft_find_matching_paren(t_list *start)
{
	t_list	*curr;
	int		depth;
	t_token	*token;

	if (!start || !start->content.token)
		return (NULL);
	
	if (start->content.token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	
	curr = start->next;
	depth = 1;
	
	while (curr && depth > 0)
	{
		token = curr->content.token;
		if (!token)
		{
			curr = curr->next;
			continue;
		}
		
		if (token->type == TOKEN_PAREN_OPEN)
			depth++;
		else if (token->type == TOKEN_PAREN_CLOSE)
			depth--;
		
		if (depth == 0)
			return (curr);
		
		curr = curr->next;
	}
	
	return (NULL); // Unmatched parenthesis
}

/**
 * @brief Validates parentheses syntax in the token list
 * 
 * @param token_h Head of token list
 * @return bool true if valid, false if syntax error
 */
bool	ft_validate_parentheses(t_list *token_h)
{
	t_list	*curr;
	int		depth;
	t_token	*token;
	t_list	*prev_significant;

	if (!token_h)
		return (true);
	
	curr = token_h;
	depth = 0;
	prev_significant = NULL;
	
	while (curr)
	{
		token = curr->content.token;
		if (!token)
		{
			curr = curr->next;
			continue;
		}
		
		if (token->type == TOKEN_PAREN_OPEN)
		{
			depth++;
			
			// Check if there's a valid token before opening paren
			if (prev_significant && 
				prev_significant->content.token->type != TOKEN_PIPE &&
				prev_significant->content.token->type != TOKEN_AND &&
				prev_significant->content.token->type != TOKEN_OR)
			{
				ft_printerr("minishell: syntax error near `('\n");
				return (false);
			}
		}
		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			depth--;
			if (depth < 0)
			{
				ft_printerr("minishell: syntax error: unexpected `)'\n");
				return (false);
			}
			
			// Check if there's something inside the parentheses
			if (prev_significant && 
				prev_significant->content.token->type == TOKEN_PAREN_OPEN)
			{
				ft_printerr("minishell: syntax error: empty parentheses\n");
				return (false);
			}
		}
		
		// Track significant tokens (non-space)
		if (token->type != TOKEN_SPACE)
			prev_significant = curr;
		
		curr = curr->next;
	}
	
	if (depth != 0)
	{
		ft_printerr("minishell: syntax error: unmatched parentheses\n");
		return (false);
	}
	
	return (true);
}

/**
 * @brief Creates a subshell environment copy
 * 
 * @param parent_env Parent environment
 * @return t_env* New environment for subshell
 */
static t_env	*ft_create_subshell_env(t_env *parent_env)
{
	t_env	*sub_env;
	int		i;
	int		count;

	if (!parent_env)
		return (NULL);
	
	sub_env = (t_env *)malloc(sizeof(t_env));
	if (!sub_env)
		return (NULL);
	
	// Copy environment variables
	count = 0;
	while (parent_env->env_vars[count])
		count++;
	
	sub_env->env_vars = (char **)malloc(sizeof(char *) * (count + 1));
	if (!sub_env->env_vars)
	{
		free(sub_env);
		return (NULL);
	}
	
	for (i = 0; i < count; i++)
	{
		sub_env->env_vars[i] = ft_strdup(parent_env->env_vars[i]);
		if (!sub_env->env_vars[i])
		{
			while (--i >= 0)
				free(sub_env->env_vars[i]);
			free(sub_env->env_vars);
			free(sub_env);
			return (NULL);
		}
	}
	sub_env->env_vars[count] = NULL;
	
	// Copy local environment
	sub_env->local_env = NULL;
	if (parent_env->local_env)
	{
		count = 0;
		while (parent_env->local_env[count])
			count++;
		
		if (count > 0)
		{
			sub_env->local_env = (char **)malloc(sizeof(char *) * (count + 1));
			if (sub_env->local_env)
			{
				for (i = 0; i < count; i++)
				{
					sub_env->local_env[i] = ft_strdup(parent_env->local_env[i]);
					if (!sub_env->local_env[i])
					{
						while (--i >= 0)
							free(sub_env->local_env[i]);
						free(sub_env->local_env);
						sub_env->local_env = NULL;
						break;
					}
				}
				if (sub_env->local_env)
					sub_env->local_env[count] = NULL;
			}
		}
	}
	
	sub_env->last_exit_code = parent_env->last_exit_code;
	return (sub_env);
}

/**
 * @brief Frees a subshell environment
 * 
 * @param env Environment to free
 */
static void	ft_free_subshell_env(t_env *env)
{
	int	i;

	if (!env)
		return;
	
	if (env->env_vars)
	{
		i = 0;
		while (env->env_vars[i])
			free(env->env_vars[i++]);
		free(env->env_vars);
	}
	
	if (env->local_env)
	{
		i = 0;
		while (env->local_env[i])
			free(env->local_env[i++]);
		free(env->local_env);
	}
	
	free(env);
}

/**
 * @brief Executes commands in a subshell (fork)
 * 
 * @param content AST node with the subshell content
 * @param parent_env Parent environment
 * @return int Exit code of the subshell
 */
int	ft_execute_subshell(t_ast_node *content, t_env *parent_env)
{
	pid_t	pid;
	int		status;
	t_env	*sub_env;
	int		exit_code;

	if (!content || !parent_env)
		return (1);
	
	pid = fork();
	if (pid == -1)
	{
		ft_printerr("minishell: fork failed\n");
		return (1);
	}
	
	if (pid == 0)
	{
		// Child process (subshell)
		sub_env = ft_create_subshell_env(parent_env);
		if (!sub_env)
			exit(1);
		
		// Execute the content in the subshell
		exit_code = ft_execute_logical(content, sub_env);
		
		ft_free_subshell_env(sub_env);
		exit(exit_code);
	}
	
	// Parent process
	waitpid(pid, &status, 0);
	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	
	return (1);
}

/**
 * @brief Parses a group (parentheses) from tokens
 * 
 * @param shell The shell structure
 * @param start Start of the group (opening paren)
 * @param end End of the group (closing paren)
 * @return t_ast_node* AST node representing the group
 */
t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end)
{
	t_ast_node	*content;
	t_ast_node	*group_node;
	t_list		*inner_start;
	t_list		*inner_end;

	if (!shell || !start || !end)
		return (NULL);
	
	if (!start->content.token || !end->content.token)
		return (NULL);
	
	if (start->content.token->type != TOKEN_PAREN_OPEN ||
		end->content.token->type != TOKEN_PAREN_CLOSE)
		return (NULL);
	
	// Get the content inside the parentheses
	inner_start = start->next;
	inner_end = end;
	
	// Skip spaces at the beginning
	while (inner_start && inner_start != inner_end && 
		   inner_start->content.token->type == TOKEN_SPACE)
		inner_start = inner_start->next;
	
	if (inner_start == inner_end)
	{
		ft_printerr("minishell: syntax error: empty parentheses\n");
		return (NULL);
	}
	
	// Parse the content inside the parentheses
	content = ft_parse_expression(shell, shell->token, inner_start, inner_end);
	if (!content)
		return (NULL);
	
	// Create the group node
	group_node = ft_create_group_node(content);
	if (!group_node)
	{
		ft_free_ast(content);
		return (NULL);
	}
	
	return (group_node);
}

/**
 * @brief Enhanced expression parsing with parentheses support
 * 
 * @param shell The shell structure
 * @param token_h Head of the token list
 * @param start Start of the expression
 * @param end End of the expression
 * @return t_ast_node* AST node representing the expression
 */
t_ast_node	*ft_parse_expression_with_parens(t_shell *shell, t_list *token_h,
                                             t_list *start, t_list *end)
{
	t_list		*curr;
	t_list		*paren_start;
	t_list		*paren_end;

	if (!shell || !start)
		return (NULL);
	
	// First, validate parentheses
	if (!ft_validate_parentheses(start))
		return (NULL);
	
	// Look for parentheses at the current level
	curr = start;
	while (curr && curr != end)
	{
		if (curr->content.token && 
			curr->content.token->type == TOKEN_PAREN_OPEN)
		{
			paren_start = curr;
			paren_end = ft_find_matching_paren(paren_start);
			
			if (!paren_end)
			{
				ft_printerr("minishell: syntax error: unmatched `('\n");
				return (NULL);
			}
			
			// If the entire expression is just a group
			if (paren_start == start && paren_end->next == end)
				return (ft_parse_group(shell, paren_start, paren_end));
			
			// Continue with normal expression parsing
			break;
		}
		curr = curr->next;
	}
	
	// If no parentheses found at this level, use normal expression parsing
	return (ft_parse_expression(shell, token_h, start, end));
}

/**
 * @brief Executes a group node (subshell)
 * 
 * @param node Group node to execute
 * @param env Environment
 * @return int Exit code
 */
int	ft_execute_group(t_ast_node *node, t_env *env)
{
	if (!node || !env || node->type != NODE_GROUP)
		return (1);
	
	if (!node->left)
		return (0);
	
	// Execute the content in a subshell
	return (ft_execute_subshell(node->left, env));
}

/**
 * @brief Enhanced main parsing function with parentheses support
 * 
 * @param shell The shell structure
 * @return int 0 on success, -1 on error
 */
int	ft_parsing_with_parentheses(t_shell *shell)
{
	t_list	*token_h;
	
	if (!shell || !shell->token)
		return (-1);
	
	token_h = shell->token;
	
	// Basic syntax validation
	if (ft_lexer_checker(shell) < 0)
		return (-1);
	
	// Validate parentheses syntax
	if (!ft_validate_parentheses(token_h))
		return (-1);
	
	// Parse with parentheses and logical operators
	shell->ast = ft_parse_expression_with_parens(shell, token_h, token_h, NULL);
	if (!shell->ast)
		return (-1);
	
	return (0);
}

/**
 * @brief Debug function to print group nodes
 * 
 * @param node AST node to print
 * @param level Indentation level
 */
void	ft_print_group_ast(t_ast_node *node, int level)
{
	int	i;

	if (!node)
		return;
	
	for (i = 0; i < level; i++)
		ft_printf("  ");
	
	if (node->type == NODE_GROUP)
	{
		ft_printf("GROUP (subshell):\n");
		if (node->left)
			ft_print_group_ast(node->left, level + 1);
	}
	else
	{
		ft_print_logical_ast(node, level);
	}
}
