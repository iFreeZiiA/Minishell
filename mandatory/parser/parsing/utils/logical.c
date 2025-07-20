/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:55:43 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Gets string representation of logical operator
 * 
 * @param type AST node type
 * @return char* String representation
 */
char	*ft_get_logical_op_str(node_type type)
{
	if (type == NODE_AND)
		return ("&&");
	if (type == NODE_OR)
		return ("||");
	if (type == NODE_PIPE)
		return ("|");
	return ("UNKNOWN");
}

/**
 * @brief Prints logical AST for debugging
 * 
 * @param node AST node to print
 * @param level Indentation level
 */
void	ft_print_logical_ast(t_ast_node *node, int level)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < level)
	{
		ft_printf("  ");
		i++;
	}
	if (node->type == NODE_AND || node->type == NODE_OR || node->type == NODE_PIPE)
	{
		ft_printf("OP: %s\n", ft_get_logical_op_str(node->type));
		ft_print_logical_ast(node->left, level + 1);
		ft_print_logical_ast(node->right, level + 1);
	}
	else
		ft_print_ast_simple(node, 0);
}

/**
 * @brief Validates logical AST structure
 * 
 * @param node AST node to validate
 * @return bool true if valid
 */
bool	ft_validate_logical_ast(t_ast_node *node)
{
	if (!node)
		return (false);
	if (node->type == NODE_AND || node->type == NODE_OR || node->type == NODE_PIPE)
	{
		if (!node->left || !node->right)
			return (false);
		return (ft_validate_logical_ast(node->left) && ft_validate_logical_ast(node->right));
	}
	return (ft_validate_ast_structure(node) == 0);
}

/**
 * @brief Counts logical operators in token list
 * 
 * @param token_h Head of token list
 * @return int Number of logical operators
 */
int	ft_count_logical_operators(t_list *token_h)
{
	t_list	*curr;
	int		count;

	if (!token_h)
		return (0);
	curr = token_h;
	count = 0;
	while (curr)
	{
		if (ft_is_logical_operator_token(curr->content.token))
			count++;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Checks if token list has logical operators
 * 
 * @param token_h Head of token list
 * @return bool true if logical operators present
 */
bool	ft_has_logical_operators(t_list *token_h)
{
	return (ft_count_logical_operators(token_h) > 0);
}
