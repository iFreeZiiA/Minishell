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

/*
 * Fonctions de validation mises en commentaire car non utilisées actuellement
 * Elles peuvent être réactivées plus tard si nécessaire
 */

/*
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

int	ft_validate_ast_structure(t_ast_node *root)
{
	t_ast_node	*visited[1000];
	int			count;
	int			max_depth;

	if (!root)
		return (1);
	// Validation commentée temporairement
	// if (!ft_validate_command_node(root) && !ft_validate_operator_node(root))
	//	return (0);
	count = 0;
	// if (!ft_check_cycles(root, visited, &count))
	//	return (0);
	// max_depth = ft_validate_depth(root, 0);
	// if (max_depth > 50)
	//	return (0);
	(void)visited;
	(void)count; 
	(void)max_depth;
	return (1);
}
*/
