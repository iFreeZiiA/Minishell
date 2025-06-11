/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_expression.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:39:07 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Handles parenthesized expressions
 * 
 * @param shell Shell structure
 * @param start Opening parenthesis token
 * @param end Ending boundary
 * @return t_ast_node* Group node or NULL on error
 */
static t_ast_node	*ft_handle_parentheses(t_shell *shell,
		t_list *start, t_list *end)
{
	t_list	*closing;

	if (start->content.token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	closing = ft_find_matching_paren(start);
	if (closing && (!end || closing->next == end))
		return (ft_parse_group(shell, start, closing));
	return (NULL);
}

/**
 * @brief Creates binary operator AST node
 * 
 * @param shell Shell structure
 * @param token_h Token list head
 * @param operator_token Operator token
 * @param start Expression start
 * @param end Expression end
 * @return t_ast_node* Binary operator node or NULL on error
 */
static t_ast_node	*ft_create_binary_operation(t_shell *shell,
		t_list *token_h, t_list *operator_token, t_list *start, t_list *end)
{
	t_ast_node	*left;
	t_ast_node	*right;
	node_type	op_type;

	op_type = ft_token_to_node_type(operator_token->content.token->type);
	left = ft_parse_expression(shell, token_h, start, operator_token);
	if (!left)
		return (NULL);
	right = ft_parse_expression(shell, token_h, operator_token->next, end);
	if (!right)
	{
		ft_free_ast(left);
		return (NULL);
	}
	return (ft_create_operator_ast(op_type, left, right));
}

/**
 * @brief Finds and processes logical operators
 * 
 * @param shell Shell structure
 * @param token_h Token list head
 * @param start Expression start
 * @param end Expression end
 * @return t_ast_node* Logical operation node or NULL if none found
 */
static t_ast_node	*ft_process_logical_operators(t_shell *shell,
		t_list *token_h, t_list *start, t_list *end)
{
	t_list	*operator_token;

	operator_token = ft_find_operator(start, end, TOKEN_OR);
	if (!operator_token)
		operator_token = ft_find_operator(start, end, TOKEN_AND);
	if (operator_token)
		return (ft_create_binary_operation(shell, token_h,
				operator_token, start, end));
	return (NULL);
}

/**
 * @brief Parses expressions with operator precedence
 * 
 * @param shell Shell structure
 * @param token_h Token list head
 * @param start Expression start
 * @param end Expression end
 * @return t_ast_node* Expression AST node or NULL on error
 */
t_ast_node	*ft_parse_expression(t_shell *shell, t_list *token_h,
		t_list *start, t_list *end)
{
	t_ast_node	*node;
	t_list		*operator_token;

	if (!shell || !start)
		return (NULL);
	node = ft_handle_parentheses(shell, start, end);
	if (node)
		return (node);
	node = ft_process_logical_operators(shell, token_h, start, end);
	if (node)
		return (node);
	operator_token = ft_find_operator(start, end, TOKEN_PIPE);
	if (operator_token)
		return (ft_create_binary_operation(shell, token_h,
				operator_token, start, end));
	return (ft_parse_simple_command(shell, start, end));
}
