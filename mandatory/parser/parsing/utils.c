/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Checks if a token is an operator token
 * 
 * This function determines if the given token represents an operator
 * (pipe, logical AND, or logical OR).
 * 
 * @param token Pointer to token to check
 * @return bool true if token is an operator, false otherwise
 */
bool	ft_is_operator_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->type == TOKEN_PIPE)
		return (true);
	if (token->type == TOKEN_AND)
		return (true);
	if (token->type == TOKEN_OR)
		return (true);
	return (false);
}

/**
 * @brief Checks if a token is a word-type token
 * 
 * This function determines if the given token represents a word, variable,
 * or wildcard that can be part of a command.
 * 
 * @param token Pointer to token to check
 * @return bool true if token is a word-type token, false otherwise
 */
bool	ft_is_word_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->type == TOKEN_WORD)
		return (true);
	if (token->type == TOKEN_VAR)
		return (true);
	if (token->type == TOKEN_WILDCARD)
		return (true);
	return (false);
}

/**
 * @brief Finds the next operator at the current parentheses depth level
 * 
 * This function searches for operators within a token range while respecting
 * parentheses nesting. It returns operators in order of precedence:
 * 1. OR operator (lowest precedence)
 * 2. AND operator (medium precedence)  
 * 3. PIPE operator (highest precedence)
 * 
 * @param start Starting token in the search range
 * @param end Ending token in the search range (exclusive)
 * @return t_list* Pointer to the first operator found at depth 0, or NULL
 */
t_list	*ft_find_operator_at_level(t_list *start, t_list *end)
{
	t_list	*curr;
	t_list	*op_or;
	t_list	*op_and;
	t_list	*op_pipe;
	int		depth;

	curr = start;
	depth = 0;
	op_or = NULL;
	op_and = NULL;
	op_pipe = NULL;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_PAREN_OPEN)
			depth++;
		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
			depth--;
		else if (depth == 0)
		{
			if (curr->content.token->type == TOKEN_OR && !op_or)
				op_or = curr;
			else if (curr->content.token->type == TOKEN_AND && !op_and)
				op_and = curr;
			else if (curr->content.token->type == TOKEN_PIPE && !op_pipe)
				op_pipe = curr;
		}
		curr = curr->next;
	}
	if (op_or)
		return (op_or);
	if (op_and)
		return (op_and);
	return (op_pipe);
}

/**
 * @brief Checks if a token type represents a redirection operator
 * 
 * This function determines if the given token type is one of the
 * redirection operators: input, output, heredoc, or append.
 * 
 * @param type Token type to check
 * @return bool true if token is a redirection operator, false otherwise
 */
bool	ft_is_redirect_token(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (true);
	if (type == TOKEN_REDIR_OUT)
		return (true);
	if (type == TOKEN_HEREDOC)
		return (true);
	if (type == TOKEN_APPEND)
		return (true);
	return (false);
}

/**
 * @brief Parses a complete expression with operator precedence handling
 * 
 * This is the main recursive function that builds the AST by:
 * 1. Handling parenthesized groups first
 * 2. Finding operators at the current nesting level
 * 3. Creating binary operator nodes with left and right children
 * 4. Parsing individual commands when no operators are found
 * 
 * @param shell Pointer to shell structure for context
 * @param token_h Head of the complete token list
 * @param start Starting position in token list for this expression
 * @param end Ending position in token list for this expression (exclusive)
 * @return t_ast_node* Root node of the parsed expression, or NULL on error
 */
t_ast_node	*ft_parse_expression(t_shell *shell, t_list *token_h,
					t_list *start, t_list *end)
{
	t_list		*op;
	t_list		*close_paren;
	t_ast_node	*left;
	t_ast_node	*right;

	if (!start || start == end)
		return (NULL);
	if (start->content.token->type == TOKEN_PAREN_OPEN)
	{
		close_paren = ft_find_matching_paren(start);
		if (close_paren && (!end || close_paren->next == end))
			return (ft_parse_group(shell, start, close_paren));
	}
	op = ft_find_operator_at_level(start, end);
	if (op)
	{
		left = ft_parse_expression(shell, token_h, start, op);
		right = ft_parse_expression(shell, token_h, op->next, end);
		if (!left || !right)
		{
			ft_free_ast(left);
			ft_free_ast(right);
			return (NULL);
		}
		return (ft_create_op_node(op->content.token->type, left, right));
	}
	return (ft_parse_command(start, end));
}
