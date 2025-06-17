/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Finds the next operator at the specified precedence level
 * 
 * This function searches for logical operators (AND, OR, PIPE) within a token
 * range while respecting parentheses nesting. It only returns operators at
 * the specified precedence level and depth 0 (not inside parentheses).
 * 
 * @param start Start of token sequence to search
 * @param end End of token sequence (exclusive)
 * @param precedence Target precedence level to find
 * @return t_list* Token containing the operator, NULL if not found
 */
t_list	*ft_find_operator_by_precedence(t_list *start, t_list *end, int precedence)
{
	t_list	*curr;
	int		paren_depth;
	int		op_precedence;

	if (!start)
		return (NULL);
	curr = start;
	paren_depth = 0;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_PAREN_OPEN)
			paren_depth++;
		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
			paren_depth--;
		else if (paren_depth == 0 && ft_is_logical_operator_token(curr->content.token))
		{
			op_precedence = ft_get_operator_precedence(curr->content.token->type);
			if (op_precedence == precedence)
				return (curr);
		}
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Parses expression with logical operators using precedence rules
 * 
 * This function implements recursive parsing of logical expressions with
 * proper operator precedence handling:
 * 1. Handles parenthesized groups first
 * 2. Searches for operators by precedence (OR=1, AND=2, PIPE=3)
 * 3. Creates binary operator nodes with left and right subtrees
 * 4. Falls back to simple command parsing when no operators found
 * 
 * @param shell Shell structure for parsing context
 * @param start Start token of expression to parse
 * @param end End token of expression (exclusive)
 * @return t_ast_node* AST node representing the parsed expression
 */
t_ast_node	*ft_parse_logical_expression(t_shell *shell, t_list *start, t_list *end)
{
	t_list		*operator_token;
	t_ast_node	*left;
	t_ast_node	*right;
	node_type	op_type;
	int			precedence;

	if (!shell || !start)
		return (NULL);
	if (start->content.token->type == TOKEN_PAREN_OPEN)
	{
		t_list *closing = ft_find_matching_paren(start);
		if (closing && (!end || closing->next == end))
			return (ft_parse_group(shell, start, closing));
	}
	precedence = 1;
	operator_token = ft_find_operator_by_precedence(start, end, precedence);
	if (!operator_token)
	{
		precedence = 2;
		operator_token = ft_find_operator_by_precedence(start, end, precedence);
	}
	if (!operator_token)
	{
		precedence = 3;
		operator_token = ft_find_operator_by_precedence(start, end, precedence);
	}
	if (operator_token)
	{
		op_type = ft_token_to_node_type(operator_token->content.token->type);
		left = ft_parse_logical_expression(shell, start, operator_token);
		if (!left)
			return (NULL);
		right = ft_parse_logical_expression(shell, operator_token->next, end);
		if (!right)
		{
			ft_free_ast(left);
			return (NULL);
		}
		return (ft_create_operator_node(op_type, left, right));
	}
	return (ft_parse_simple_command(shell, start, end));
}

/**
 * @brief Counts logical operators in token list for debugging analysis
 * 
 * This helper function traverses the token list and counts occurrences
 * of each type of logical operator for debugging and analysis purposes.
 * 
 * @param token_h Head of token list to analyze
 * @param and_count Pointer to store AND operator count
 * @param or_count Pointer to store OR operator count
 * @param pipe_count Pointer to store PIPE operator count
 */
static void	ft_count_logical_operators(t_list *token_h, int *and_count, 
									   int *or_count, int *pipe_count)
{
	t_list	*curr;

	if (!token_h || !and_count || !or_count || !pipe_count)
		return;
	curr = token_h;
	*and_count = 0;
	*or_count = 0;
	*pipe_count = 0;
	while (curr)
	{
		if (curr->content.token->type == TOKEN_AND)
			(*and_count)++;
		else if (curr->content.token->type == TOKEN_OR)
			(*or_count)++;
		else if (curr->content.token->type == TOKEN_PIPE)
			(*pipe_count)++;
		curr = curr->next;
	}
}

/**
 * @brief Debug function to analyze and display logical operators statistics
 * 
 * This debugging function provides detailed analysis of logical operators
 * in the token list, including counts of each operator type and syntax
 * validation status. Useful for troubleshooting parsing issues.
 * 
 * @param shell Shell structure containing the token list to analyze
 */
void	ft_debug_logical_operators(t_shell *shell)
{
	int	and_count;
	int	or_count;
	int	pipe_count;

	if (!shell || !shell->token)
		return;
	ft_count_logical_operators(shell->token, &and_count, &or_count, &pipe_count);
	ft_printf("=== LOGICAL OPERATORS ANALYSIS ===\n");
	ft_printf("AND operators (&&): %d\n", and_count);
	ft_printf("OR operators (||): %d\n", or_count);
	ft_printf("PIPE operators (|): %d\n", pipe_count);
	ft_printf("Total operators: %d\n", and_count + or_count + pipe_count);
	ft_printf("Syntax valid: %s\n", 
		ft_validate_logical_syntax(shell->token) ? "YES" : "NO");
	ft_printf("==================================\n\n");
}
