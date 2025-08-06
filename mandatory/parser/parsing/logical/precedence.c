/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precedence.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:53:16 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

t_list	*ft_find_operator_by_precedence(t_list *start, t_list *end,
		int precedence)
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

static t_list	*ft_find_operator_at_precedence(t_list *start, t_list *end)
{
	t_list	*op_token;

	op_token = ft_find_operator_by_precedence(start, end, 1);
	if (!op_token)
		op_token = ft_find_operator_by_precedence(start, end, 2);
	if (!op_token)
		op_token = ft_find_operator_by_precedence(start, end, 3);
	return (op_token);
}

static t_ast_node	*ft_create_op_node(node_type op_type, t_ast_node *left,
		t_ast_node *right)
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

t_ast_node	*ft_parse_logical_expression(t_shell *shell, t_list *start,
		t_list *end)
{
	t_list		*op_token;
	t_ast_node	*left;
	t_ast_node	*right;
	node_type	op_type;

	if (!start || !end)
		return (NULL);
	op_token = ft_find_operator_at_precedence(start, end);
	if (!op_token)
		return (ft_parse_simple_command_precedence(shell, start, end));
	left = ft_parse_logical_expression(shell, start, op_token->prev);
	right = ft_parse_logical_expression(shell, op_token->next, end);
	if (!left || !right)
		return (NULL);
	op_type = ft_token_to_node_type(op_token->content.token->type);
	return (ft_create_op_node(op_type, left, right));
}

t_ast_node	*ft_parse_simple_command_precedence(t_shell *shell, t_list *start,
		t_list *end)
{
	t_list		*sub_tokens;
	t_ast_node	*result;

	sub_tokens = ft_create_token_sublist_precedence(start, end);
	if (!sub_tokens)
		return (NULL);
	result = ft_parser(sub_tokens, shell);
	ft_free_token_sublist_precedence(sub_tokens);
	return (result);
}
