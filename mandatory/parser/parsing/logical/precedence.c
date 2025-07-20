/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precedence.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:55:29 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Converts token type to AST node type
 * 
 * @param token_type Token type to convert
 * @return node_type Corresponding AST node type
 */
node_type	ft_token_to_node_type(t_token_type token_type)
{
	if (token_type == TOKEN_PIPE)
		return (NODE_PIPE);
	if (token_type == TOKEN_AND)
		return (NODE_AND);
	if (token_type == TOKEN_OR)
		return (NODE_OR);
	return (NODE_COMMAND);
}

/**
 * @brief Finds operator with specific precedence
 * 
 * @param start Start of token range
 * @param end End of token range
 * @param precedence Target precedence level
 * @return t_list* Operator token or NULL
 */
t_list	*ft_find_operator_by_precedence(t_list *start, t_list *end, int precedence)
{
	t_list	*curr;
	t_token	*token;
	int		current_prec;

	if (!start || !end)
		return (NULL);
	curr = start;
	while (curr && curr != end->next)
	{
		token = curr->content.token;
		if (ft_is_logical_operator_token(token) || token->type == TOKEN_PIPE)
		{
			current_prec = ft_get_operator_precedence(token->type);
			if (current_prec == precedence)
				return (curr);
		}
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Creates operator AST node
 * 
 * @param op_type Operator type
 * @param left Left child
 * @param right Right child
 * @return t_ast_node* New operator node
 */
static t_ast_node	*ft_create_op_node(node_type op_type, t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = op_type;
	node->data = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

/**
 * @brief Parses expression with operator precedence
 * 
 * @param shell Shell structure
 * @param start Start token
 * @param end End token
 * @return t_ast_node* Parsed expression AST
 */
t_ast_node	*ft_parse_logical_expression(t_shell *shell, t_list *start, t_list *end)
{
	t_list		*op_token;
	t_ast_node	*left;
	t_ast_node	*right;
	node_type	op_type;

	if (!start || !end)
		return (NULL);
	op_token = ft_find_operator_by_precedence(start, end, 1);
	if (!op_token)
		op_token = ft_find_operator_by_precedence(start, end, 2);
	if (!op_token)
		op_token = ft_find_operator_by_precedence(start, end, 3);
	if (!op_token)
		return (ft_parser(start, shell));
	left = ft_parse_logical_expression(shell, start, op_token->prev);
	right = ft_parse_logical_expression(shell, op_token->next, end);
	if (!left || !right)
		return (NULL);
	op_type = ft_token_to_node_type(op_token->content.token->type);
	return (ft_create_op_node(op_type, left, right));
}
