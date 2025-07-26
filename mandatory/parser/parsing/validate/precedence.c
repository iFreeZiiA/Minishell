/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precedence.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/26 00:00:00 by user              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Gets precedence level for token type
 * 
 * @param type Token type
 * @return int Precedence level (higher = higher priority)
 */
static int	ft_get_precedence(t_token_type type)
{
	if (type == TOKEN_PAREN_OPEN || type == TOKEN_PAREN_CLOSE)
		return (4);
	if (type == TOKEN_PIPE)
		return (3);
	if (type == TOKEN_AND)
		return (2);
	if (type == TOKEN_OR)
		return (1);
	return (0);
}

/**
 * @brief Finds lowest precedence operator in token range
 * 
 * @param start Start of token range
 * @param end End of token range
 * @return t_list* Token with lowest precedence operator
 */
static t_list	*ft_find_lowest_precedence(t_list *start, t_list *end)
{
	t_list	*current;
	t_list	*lowest_op;
	t_token	*token;
	int		min_precedence;
	int		current_prec;

	current = start;
	lowest_op = NULL;
	min_precedence = 5;
	while (current && current != end)
	{
		token = (t_token *)current->content.token;
		current_prec = ft_get_precedence(token->type);
		if (current_prec > 0 && current_prec < min_precedence)
		{
			min_precedence = current_prec;
			lowest_op = current;
		}
		current = current->next;
	}
	return (lowest_op);
}

/**
 * @brief Creates binary operator AST node
 * 
 * @param op_token Operator token
 * @param left Left child node
 * @param right Right child node
 * @return t_ast_node* New binary operator node
 */
static t_ast_node	*ft_create_binary_node(t_list *op_token, t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;
	t_token		*token;

	token = (t_token *)op_token->content.token;
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	if (token->type == TOKEN_PIPE)
		node->type = NODE_PIPE;
	else if (token->type == TOKEN_AND)
		node->type = NODE_AND;
	else if (token->type == TOKEN_OR)
		node->type = NODE_OR;
	else
		node->type = NODE_COMMAND;
	node->left = left;
	node->right = right;
	node->data = NULL;
	return (node);
}

/**
 * @brief Handles parentheses grouping in expression
 * 
 * @param tokens Token list
 * @param shell Shell structure
 * @return t_ast_node* Parsed expression within parentheses
 */
static t_ast_node	*ft_handle_parentheses(t_list *tokens, t_shell *shell)
{
	t_list	*start;
	t_list	*end;
	t_token	*token;

	start = tokens;
	end = NULL;
	while (start)
	{
		token = (t_token *)start->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
		{
			end = start->next;
			while (end)
			{
				token = (t_token *)end->content.token;
				if (token->type == TOKEN_PAREN_CLOSE)
					break;
				end = end->next;
			}
			if (end)
				return (ft_parse_expression(start->next, end, shell));
		}
		start = start->next;
	}
	return (NULL);
}

/**
 * @brief Main expression parser with precedence handling
 * 
 * @param start Start of token range
 * @param end End of token range
 * @param shell Shell structure
 * @return t_ast_node* Parsed expression AST
 */
t_ast_node	*ft_parse_expression(t_list *start, t_list *end, t_shell *shell)
{
	t_list		*op_token;
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*paren_result;

	if (!start || start == end)
		return (NULL);
	paren_result = ft_handle_parentheses(start, shell);
	if (paren_result)
		return (paren_result);
	op_token = ft_find_lowest_precedence(start, end);
	if (!op_token)
		return (ft_parser(start, shell));
	left = ft_parse_expression(start, op_token, shell);
	right = ft_parse_expression(op_token->next, end, shell);
	if (!left || !right)
		return (NULL);
	return (ft_create_binary_node(op_token, left, right));
}
