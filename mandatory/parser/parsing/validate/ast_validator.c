/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/26 00:00:00 by user              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Validates command node data integrity
 * 
 * @param node Command node to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	ft_validate_command_node(t_ast_node *node)
{
	t_command	*cmd;

	if (!node || node->type != NODE_COMMAND)
		return (0);
	cmd = (t_command *)node->data;
	if (!cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
		return (0);
	return (1);
}

/**
 * @brief Validates operator node children
 * 
 * @param node Operator node to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	ft_validate_operator_node(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type != NODE_PIPE && node->type != NODE_AND 
		&& node->type != NODE_OR)
		return (1);
	if (!node->left || !node->right)
		return (0);
	return (1);
}

/**
 * @brief Checks AST for circular references
 * 
 * @param node Current node
 * @param visited Array of visited nodes
 * @param count Current visit count
 * @return int 1 if no cycles, 0 if cycle detected
 */
static int	ft_check_cycles(t_ast_node *node, t_ast_node **visited, int *count)
{
	int	i;

	if (!node || *count >= 1000)
		return (1);
	i = 0;
	while (i < *count)
	{
		if (visited[i] == node)
			return (0);
		i++;
	}
	visited[*count] = node;
	(*count)++;
	if (!ft_check_cycles(node->left, visited, count))
		return (0);
	if (!ft_check_cycles(node->right, visited, count))
		return (0);
	return (1);
}

/**
 * @brief Validates AST depth to prevent stack overflow
 * 
 * @param node Current node
 * @param depth Current depth
 * @return int Maximum depth found
 */
static int	ft_validate_depth(t_ast_node *node, int depth)
{
	int	left_depth;
	int	right_depth;

	if (!node || depth > 100)
		return (depth);
	left_depth = ft_validate_depth(node->left, depth + 1);
	right_depth = ft_validate_depth(node->right, depth + 1);
	if (left_depth > right_depth)
		return (left_depth);
	return (right_depth);
}

/**
 * @brief Main AST structure validation function
 * 
 * @param root Root node of AST
 * @return int 1 if valid structure, 0 if invalid
 */
int	ft_validate_ast_structure(t_ast_node *root)
{
	t_ast_node	*visited[1000];
	int			count;
	int			max_depth;

	if (!root)
		return (1);
	if (!ft_validate_command_node(root) && !ft_validate_operator_node(root))
		return (0);
	count = 0;
	if (!ft_check_cycles(root, visited, &count))
		return (0);
	max_depth = ft_validate_depth(root, 0);
	if (max_depth > 50)
		return (0);
	return (1);
}
