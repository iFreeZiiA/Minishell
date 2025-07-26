/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_optimizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/26 00:00:00 by user              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Checks if AST node has valid structure
 * 
 * @param node Node to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	ft_validate_node_structure(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		if (!node->data)
			return (0);
		return (1);
	}
	if (node->type == NODE_PIPE || node->type == NODE_AND 
		|| node->type == NODE_OR)
	{
		if (!node->left || !node->right)
			return (0);
		return (1);
	}
	return (1);
}

/**
 * @brief Removes unnecessary group nodes from AST
 * 
 * @param node Node to process
 * @return t_ast_node* Optimized node
 */
static t_ast_node	*ft_flatten_groups(t_ast_node *node)
{
	t_ast_node	*result;

	if (!node)
		return (NULL);
	if (node->type == NODE_GROUP && node->left && !node->right)
	{
		result = node->left;
		free(node);
		return (ft_flatten_groups(result));
	}
	if (node->left)
		node->left = ft_flatten_groups(node->left);
	if (node->right)
		node->right = ft_flatten_groups(node->right);
	return (node);
}

/**
 * @brief Optimizes sequential commands in AST
 * 
 * @param node Node to optimize
 * @return t_ast_node* Optimized node
 */
static t_ast_node	*ft_optimize_sequences(t_ast_node *node)
{
	if (!node)
		return (NULL);
	if (node->type == NODE_PIPE && node->left && node->right)
	{
		if (node->left->type == NODE_COMMAND 
			&& node->right->type == NODE_COMMAND)
		{
			return (node);
		}
	}
	if (node->left)
		node->left = ft_optimize_sequences(node->left);
	if (node->right)
		node->right = ft_optimize_sequences(node->right);
	return (node);
}

/**
 * @brief Counts total nodes in AST
 * 
 * @param node Root node
 * @return int Number of nodes
 */
static int	ft_count_nodes(t_ast_node *node)
{
	int	count;

	if (!node)
		return (0);
	count = 1;
	count += ft_count_nodes(node->left);
	count += ft_count_nodes(node->right);
	return (count);
}

/**
 * @brief Main AST optimization function
 * 
 * @param root Root node of AST
 * @return t_ast_node* Optimized AST root
 */
t_ast_node	*ft_optimize_ast(t_ast_node *root)
{
	t_ast_node	*optimized;
	int			original_count;
	int			optimized_count;

	if (!root)
		return (NULL);
	if (!ft_validate_node_structure(root))
		return (NULL);
	original_count = ft_count_nodes(root);
	optimized = ft_flatten_groups(root);
	optimized = ft_optimize_sequences(optimized);
	optimized_count = ft_count_nodes(optimized);
	if (optimized_count > original_count)
		return (root);
	return (optimized);
}
