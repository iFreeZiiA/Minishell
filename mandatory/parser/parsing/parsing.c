/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:41:24 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 19:24:30 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Creates a new AST node
 * 
 * @param type Type of the node
 * @param data Data associated with the node
 * @param left Left subtree
 * @param right Right subtree
 * @return t_ast_node* New AST node, NULL on error
 */
static t_ast_node	*ft_create_ast_node(node_type type, void *data, 
                                        t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	
	node->type = type;
	node->data = data;
	node->left = left;
	node->right = right;
	
	return (node);
}

/**
 * @brief Creates a command structure from tokens
 * 
 * @param shell The shell structure
 * @param start First token of the command
 * @param end Last token of the command
 * @return t_command* New command structure, NULL on error
 */
static t_command	*ft_create_command(t_shell *shell, t_list *start, t_list *end)
{
	t_command	*cmd;
	t_list		*curr;
	int			arg_count;
	int			i;
	
	if (!shell || !start)
		return (NULL);
	
	/* Create the command structure */
	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	
	/* Initialize the command structure */
	cmd->redirs = NULL;
	
	/* Count the number of arguments */
	arg_count = 0;
	curr = start;
	while (curr && curr != end && curr->content.token->type == TOKEN_WORD)
	{
		arg_count++;
		curr = curr->next;
	}
	
	/* Allocate memory for arguments */
	cmd->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	
	/* Copy the arguments */
	i = 0;
	curr = start;
	while (i < arg_count)
	{
		cmd->args[i] = ft_strdup(curr->content.token->value);
		if (!cmd->args[i])
		{
			/* Clean up in case of error */
			while (--i >= 0)
				free(cmd->args[i]);
			free(cmd->args);
			free(cmd);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	cmd->args[arg_count] = NULL;
	
	return (cmd);
}

/**
 * @brief Parses redirections from tokens
 * 
 * @param shell The shell structure
 * @param token_h Head of the token list
 * @param cmd Command structure to add redirections to
 * @return int 0 on success, -1 on error
 */
static int	ft_parse_redirections(t_shell *shell, t_list *token_h, t_command *cmd)
{
	t_list	*curr;
	t_redir	*redir;
	
	if (!shell || !token_h || !cmd)
		return (-1);
	
	curr = token_h;
	while (curr)
	{
		if (curr->content.token->type == TOKEN_REDIR_IN || 
			curr->content.token->type == TOKEN_REDIR_OUT ||
			curr->content.token->type == TOKEN_HEREDOC ||
			curr->content.token->type == TOKEN_APPEND)
		{
			/* Create a redirection structure */
			redir = (t_redir *)malloc(sizeof(t_redir));
			if (!redir)
				return (-1);
			
			/* Set redirection type */
			if (curr->content.token->type == TOKEN_REDIR_IN)
				redir->type = REDIR_IN;
			else if (curr->content.token->type == TOKEN_REDIR_OUT)
				redir->type = REDIR_OUT;
			else if (curr->content.token->type == TOKEN_HEREDOC)
				redir->type = REDIR_HEREDOC;
			else
				redir->type = REDIR_APPEND;
			
			/* Get the file/delimiter */
			if (curr->next && curr->next->content.token->type == TOKEN_WORD)
			{
				redir->file = ft_strdup(curr->next->content.token->value);
				if (!redir->file)
				{
					free(redir);
					return (-1);
				}
			}
			else
			{
				free(redir);
				return (-1);
			}
			
			/* Add the redirection to the command */
			redir->fd = -1;  /* Will be set during execution */
			
			/* Add to the list */
			if (!ft_lstadd_back(&cmd->redirs, 
				(t_content){.redir = redir}, TYPE_REDIR))
			{
				free(redir->file);
				free(redir);
				return (-1);
			}
			
			/* Skip the file/delimiter token */
			curr = curr->next;
		}
		
		curr = curr->next;
	}
	
	return (0);
}

/**
 * @brief Parses a pipe sequence from tokens
 * 
 * @param shell The shell structure
 * @param token_h Head of the token list
 * @param start Start of the pipe sequence
 * @param end End of the pipe sequence
 * @return t_ast_node* AST node representing the pipe sequence, NULL on error
 */
static t_ast_node	*ft_parse_pipe(t_shell *shell, t_list *token_h, 
                                  t_list *start, t_list *end)
{
	t_list		*curr;
	t_list		*pipe_token;
	t_ast_node	*left;
	t_ast_node	*right;
	
	if (!shell || !token_h || !start)
		return (NULL);
	
	/* Find the pipe token */
	curr = start;
	pipe_token = NULL;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_PIPE)
		{
			pipe_token = curr;
			break;
		}
		curr = curr->next;
	}
	
	/* If no pipe is found, parse as a command */
	if (!pipe_token)
	{
		t_command *cmd = ft_create_command(shell, start, end);
		if (!cmd)
			return (NULL);
			
		/* Parse redirections */
		if (ft_parse_redirections(shell, start, cmd) < 0)
		{
			/* Clean up command */
			int i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
			free(cmd);
			return (NULL);
		}
		
		return (ft_create_ast_node(NODE_COMMAND, cmd, NULL, NULL));
	}
	
	/* Parse left side of the pipe */
	left = ft_parse_pipe(shell, token_h, start, pipe_token);
	if (!left)
		return (NULL);
	
	/* Parse right side of the pipe */
	right = ft_parse_pipe(shell, token_h, pipe_token->next, end);
	if (!right)
	{
		/* Clean up left side */
		free(left);
		return (NULL);
	}
	
	/* Create the pipe node */
	return (ft_create_ast_node(NODE_PIPE, NULL, left, right));
}

/**
 * @brief Parses a logical operator (AND, OR) from tokens
 * 
 * @param shell The shell structure
 * @param token_h Head of the token list
 * @param start Start of the logical expression
 * @param end End of the logical expression
 * @return t_ast_node* AST node representing the logical expression, NULL on error
 */
static t_ast_node	*ft_parse_logical(t_shell *shell, t_list *token_h, 
                                     t_list *start, t_list *end)
{
	t_list		*curr;
	t_list		*operator_token;
	t_ast_node	*left;
	t_ast_node	*right;
	node_type	op_type;
	
	if (!shell || !token_h || !start)
		return (NULL);
	
	/* Find the logical operator token (AND, OR) */
	curr = start;
	operator_token = NULL;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_AND || 
			curr->content.token->type == TOKEN_OR)
		{
			operator_token = curr;
			break;
		}
		curr = curr->next;
	}
	
	/* If no operator is found, parse as a pipe sequence */
	if (!operator_token)
		return (ft_parse_pipe(shell, token_h, start, end));
	
	/* Set operator type */
	op_type = (operator_token->content.token->type == TOKEN_AND) ? 
				NODE_AND : NODE_OR;
	
	/* Parse left side of the operator */
	left = ft_parse_logical(shell, token_h, start, operator_token);
	if (!left)
		return (NULL);
	
	/* Parse right side of the operator */
	right = ft_parse_logical(shell, token_h, operator_token->next, end);
	if (!right)
	{
		/* Clean up left side */
		free(left);
		return (NULL);
	}
	
	/* Create the operator node */
	return (ft_create_ast_node(op_type, NULL, left, right));
}

/**
 * @brief Parses a group (parentheses) from tokens
 * 
 * @param shell The shell structure
 * @param token_h Head of the token list
 * @param start Start of the group
 * @param end End of the group
 * @return t_ast_node* AST node representing the group, NULL on error
 */
// static t_ast_node	*ft_parse_group(t_shell *shell, t_list *token_h, 
//                                    t_list *start, t_list *end)
// {
// 	t_ast_node	*content;
	
// 	if (!shell || !token_h || !start || !end || 
// 		start->content.token->type != TOKEN_PAREN_OPEN || 
// 		end->content.token->type != TOKEN_PAREN_CLOSE)
// 		return (NULL);
	
// 	/* Parse the content inside the parentheses */
// 	content = ft_parse_logical(shell, token_h, start->next, end);
// 	if (!content)
// 		return (NULL);
	
// 	/* Create the group node */
// 	return (ft_create_ast_node(NODE_GROUP, NULL, content, NULL));
// }

/**
 * @brief Finds matching closing parenthesis
 * 
 * @param start Token with opening parenthesis
 * @return t_list* Token with matching closing parenthesis, NULL if not found
 */
// static t_list	*ft_find_matching_paren(t_list *start)
// {
// 	t_list	*curr;
// 	int		depth;
	
// 	if (!start || start->content.token->type != TOKEN_PAREN_OPEN)
// 		return (NULL);
	
// 	curr = start->next;
// 	depth = 1;
// 	while (curr && depth > 0)
// 	{
// 		if (curr->content.token->type == TOKEN_PAREN_OPEN)
// 			depth++;
// 		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
// 			depth--;
		
// 		if (depth == 0)
// 			return (curr);
		
// 		curr = curr->next;
// 	}
	
// 	return (NULL);
// }

/**
 * @brief Main parsing function to build the AST
 * 
 * @param shell The shell structure
 * @return int 0 on success, -1 on error
 */
int	ft_parsing(t_shell *shell)
{
	t_list	*token_h;
	
	if (!shell || !shell->token)
		return (-1);
	
	token_h = shell->token;
	
	/* Check for syntax errors */
	if (ft_lexer_checker(shell) < 0)
		return (-1);
	
	/* Build the AST */
	shell->ast = ft_parse_logical(shell, token_h, token_h, NULL);
	if (!shell->ast)
		return (-1);
	
	return (0);
}