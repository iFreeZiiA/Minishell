/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:41:24 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/11 11:31:44 by jjorda           ###   ########.fr       */
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
	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redirs = NULL;
	arg_count = 0;
	curr = start;
	while (curr && curr != end && curr->content.token->type == TOKEN_WORD)
	{
		arg_count++;
		curr = curr->next;
	}
	cmd->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	curr = start;
	while (i < arg_count)
	{
		cmd->args[i] = ft_strdup(curr->content.token->value);
		if (!cmd->args[i])
		{
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
			redir = (t_redir *)malloc(sizeof(t_redir));
			if (!redir)
				return (-1);
			if (curr->content.token->type == TOKEN_REDIR_IN)
				redir->type = REDIR_IN;
			else if (curr->content.token->type == TOKEN_REDIR_OUT)
				redir->type = REDIR_OUT;
			else if (curr->content.token->type == TOKEN_HEREDOC)
				redir->type = REDIR_HEREDOC;
			else
				redir->type = REDIR_APPEND;
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
			redir->fd = -1;
			if (!ft_lstadd_back(&cmd->redirs, 
				(t_content){.redir = redir}, TYPE_REDIR))
			{
				free(redir->file);
				free(redir);
				return (-1);
			}
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
	if (!pipe_token)
	{
		t_command *cmd = ft_create_command(shell, start, end);
		if (!cmd)
			return (NULL);
		if (ft_parse_redirections(shell, start, cmd) < 0)
		{
			int i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
			free(cmd);
			return (NULL);
		}
		return (ft_create_ast_node(NODE_COMMAND, cmd, NULL, NULL));
	}
	left = ft_parse_pipe(shell, token_h, start, pipe_token);
	if (!left)
		return (NULL);
	right = ft_parse_pipe(shell, token_h, pipe_token->next, end);
	if (!right)
	{
		free(left);
		return (NULL);
	}
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
	if (!operator_token)
		return (ft_parse_pipe(shell, token_h, start, end));
	op_type = (operator_token->content.token->type == TOKEN_AND) ? 
				NODE_AND : NODE_OR;
	left = ft_parse_logical(shell, token_h, start, operator_token);
	if (!left)
		return (NULL);
	right = ft_parse_logical(shell, token_h, operator_token->next, end);
	if (!right)
	{
		free(left);
		return (NULL);
	}
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
	if (ft_lexer_checker(shell) < 0)
		return (-1);
	shell->ast = ft_parse_logical(shell, token_h, token_h, NULL);
	if (!shell->ast)
		return (-1);
	return (0);
}