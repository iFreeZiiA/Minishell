/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:41:24 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/23 23:08:16 by jjorda           ###   ########.fr       */
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
	
	// Count arguments (skip redirections)
	arg_count = 0;
	curr = start;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_WORD)
			arg_count++;
		else if (curr->content.token->type == TOKEN_REDIR_IN || 
			curr->content.token->type == TOKEN_REDIR_OUT ||
			curr->content.token->type == TOKEN_HEREDOC ||
			curr->content.token->type == TOKEN_APPEND)
		{
			// Skip redirection operator and filename
			curr = curr->next;
		}
		if (curr)
			curr = curr->next;
	}
	
	cmd->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	
	// Fill arguments
	i = 0;
	curr = start;
	while (curr && curr != end && i < arg_count)
	{
		if (curr->content.token->type == TOKEN_WORD)
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
		}
		else if (curr->content.token->type == TOKEN_REDIR_IN || 
			curr->content.token->type == TOKEN_REDIR_OUT ||
			curr->content.token->type == TOKEN_HEREDOC ||
			curr->content.token->type == TOKEN_APPEND)
		{
			// Skip redirection operator and filename
			curr = curr->next;
		}
		if (curr)
			curr = curr->next;
	}
	cmd->args[arg_count] = NULL;
	return (cmd);
}

/**
 * @brief Parses a simple command (no operators)
 * 
 * @param shell The shell structure
 * @param start Start of the command
 * @param end End of the command
 * @return t_ast_node* AST node representing the command
 */
t_ast_node	*ft_parse_simple_command(t_shell *shell, t_list *start, t_list *end)
{
	t_command	*cmd;

	if (!shell || !start)
		return (NULL);
	
	cmd = ft_create_command(shell, start, end);
	if (!cmd)
		return (NULL);
	
	// Parse redirections
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

/**
 * @brief Finds the next operator at the current precedence level
 * 
 * @param start Start of token sequence
 * @param end End of token sequence
 * @param target_type Type of operator to find
 * @return t_list* Token containing the operator, NULL if not found
 */
static t_list	*ft_find_operator(t_list *start, t_list *end, t_token_type target_type)
{
	t_list	*curr;
	int		paren_depth;

	curr = start;
	paren_depth = 0;
	
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_PAREN_OPEN)
			paren_depth++;
		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
			paren_depth--;
		else if (paren_depth == 0 && curr->content.token->type == target_type)
			return (curr);
		
		curr = curr->next;
	}
	
	return (NULL);
}

/**
 * @brief Parses expressions with operator precedence
 * 
 * @param shell The shell structure
 * @param token_h Head of the token list
 * @param start Start of the expression
 * @param end End of the expression
 * @return t_ast_node* AST node representing the expression
 */
t_ast_node	*ft_parse_expression(t_shell *shell, t_list *token_h, 
                                  t_list *start, t_list *end)
{
	t_list		*operator_token;
	t_ast_node	*left;
	t_ast_node	*right;
	node_type	op_type;

	if (!shell || !start)
		return (NULL);
	
	// Handle parentheses first
	if (start->content.token->type == TOKEN_PAREN_OPEN)
	{
		t_list *closing = ft_find_matching_paren(start);
		if (closing && (!end || closing->next == end))
			return (ft_parse_group(shell, start, closing));
	}
	
	// Look for logical operators (lowest precedence)
	operator_token = ft_find_operator(start, end, TOKEN_OR);
	if (!operator_token)
		operator_token = ft_find_operator(start, end, TOKEN_AND);
	
	if (operator_token)
	{
		op_type = (operator_token->content.token->type == TOKEN_AND) ? 
					NODE_AND : NODE_OR;
		
		left = ft_parse_expression(shell, token_h, start, operator_token);
		if (!left)
			return (NULL);
		
		right = ft_parse_expression(shell, token_h, operator_token->next, end);
		if (!right)
		{
			ft_free_ast(left);
			return (NULL);
		}
		
		return (ft_create_ast_node(op_type, NULL, left, right));
	}
	
	// Look for pipe (higher precedence than logical operators)
	operator_token = ft_find_operator(start, end, TOKEN_PIPE);
	if (operator_token)
	{
		left = ft_parse_expression(shell, token_h, start, operator_token);
		if (!left)
			return (NULL);
		
		right = ft_parse_expression(shell, token_h, operator_token->next, end);
		if (!right)
		{
			ft_free_ast(left);
			return (NULL);
		}
		
		return (ft_create_ast_node(NODE_PIPE, NULL, left, right));
	}
	
	// No operators found, parse as simple command
	return (ft_parse_simple_command(shell, start, end));
}

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
	
	// Basic syntax validation
	if (ft_lexer_checker(shell) < 0)
		return (-1);
	
	// Validate parentheses syntax
	if (!ft_validate_parentheses(token_h))
		return (-1);
	
	// Expand wildcards
	ft_exp_wildcard(shell, &token_h);
	shell->token = token_h;
	
	// Parse with full support for parentheses and logical operators
	shell->ast = ft_parse_expression(shell, token_h, token_h, NULL);
	if (!shell->ast)
		return (-1);
	
	return (0);
}
