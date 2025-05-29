/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_print_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:42:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/29 18:16:22 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Prints indentation for debug output
 * 
 * @param level Indentation level
 */
static void	ft_print_indent(int level)
{
	int	i;

	i = 0;
	while (i < level)
	{
		ft_printf("  ");
		i++;
	}
}

/**
 * @brief Prints operator node for debugging
 * 
 * @param node Operator node
 * @param level Indentation level
 */
void	ft_print_operator_node(t_ast_node *node, int level)
{
	ft_printf("OPERATOR: %s\n", ft_get_logical_op_str(node->type));
	if (node->left)
	{
		ft_print_indent(level + 1);
		ft_printf("LEFT:\n");
		ft_print_logical_ast(node->left, level + 2);
	}
	if (node->right)
	{
		ft_print_indent(level + 1);
		ft_printf("RIGHT:\n");
		ft_print_logical_ast(node->right, level + 2);
	}
}

/**
 * @brief Prints command node for debugging
 * 
 * @param node Command node
 */
void	ft_print_command_node(t_ast_node *node)
{
	t_command	*cmd;
	int			i;

	cmd = (t_command *)node->data;
	ft_printf("COMMAND: ");
	if (cmd && cmd->args && cmd->args[0])
	{
		i = 0;
		while (cmd->args[i])
		{
			ft_printf("%s", cmd->args[i]);
			if (cmd->args[i + 1])
				ft_printf(" ");
			i++;
		}
	}
	ft_printf("\n");
}

/**
 * @brief Prints group node for debugging
 * 
 * @param node Group node
 * @param level Indentation level
 */
void	ft_print_group_node(t_ast_node *node, int level)
{
	ft_printf("GROUP (subshell):\n");
	if (node->left)
		ft_print_logical_ast(node->left, level + 1);
}

/**
 * @brief Gets logical operator string representation
 * 
 * @param type Node type
 * @return char* String representation
 */
char	*ft_get_logical_op_str(node_type type)
{
	if (type == NODE_AND)
		return ("&&");
	else if (type == NODE_OR)
		return ("||");
	else if (type == NODE_PIPE)
		return ("|");
	else
		return ("UNKNOWN");
}
