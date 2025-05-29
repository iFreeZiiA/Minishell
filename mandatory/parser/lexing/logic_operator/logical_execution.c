/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/29 18:15:10 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Checks if a command is a builtin
 * 
 * @param cmd Command name to check
 * @return bool true if builtin, false otherwise
 */
bool	ft_is_builtin_cmd(const char *cmd)
{
	if (!cmd)
		return (false);
	return (!ft_strcmp(cmd, "echo") ||
			!ft_strcmp(cmd, "cd") ||
			!ft_strcmp(cmd, "pwd") ||
			!ft_strcmp(cmd, "export") ||
			!ft_strcmp(cmd, "unset") ||
			!ft_strcmp(cmd, "env") ||
			!ft_strcmp(cmd, "exit"));
}

/**
 * @brief Executes logical operators (&& and ||) with short-circuit evaluation
 * 
 * @param node AST node containing the logical operator
 * @param env Environment structure
 * @return int Final exit code
 */
int	ft_execute_logical(t_ast_node *node, t_env *env)
{
	int	left_exit_code;
	int	right_exit_code;

	if (!node || !env)
		return (1);
	if (node->type != NODE_AND && node->type != NODE_OR)
		return (ft_execute_node(node, env));
	if (!node->left)
		return (1);
	left_exit_code = ft_execute_logical(node->left, env);
	env->last_exit_code = left_exit_code;
	if (node->type == NODE_AND)
	{
		if (left_exit_code == 0 && node->right)
		{
			right_exit_code = ft_execute_logical(node->right, env);
			env->last_exit_code = right_exit_code;
			return (right_exit_code);
		}
		return (left_exit_code);
	}
	return (ft_handle_or_operator(node, env, left_exit_code));
}

/**
 * @brief Main function to execute an AST with logical operators
 * 
 * @param shell Shell structure
 * @return int Exit code of the execution
 */
int	ft_execute_ast(t_shell *shell)
{
	int	exit_code;

	if (!shell || !shell->ast || !shell->env)
		return (1);
	exit_code = ft_execute_logical(shell->ast, shell->env);
	shell->env->last_exit_code = exit_code;
	return (exit_code);
}

/**
 * @brief Prints logical operator for debugging
 * 
 * @param type Node type
 * @return char* String representation
 */
static char	*ft_get_logical_op_str(node_type type)
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

/**
 * @brief Debug function to print logical expression
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
	if (node->type == NODE_AND || node->type == NODE_OR 
		|| node->type == NODE_PIPE)
		ft_print_operator_node(node, level);
	else if (node->type == NODE_COMMAND)
		ft_print_command_node(node);
	else if (node->type == NODE_GROUP)
		ft_print_group_node(node, level);
	else
		ft_printf("NODE_TYPE: %d\n", node->type);
}
