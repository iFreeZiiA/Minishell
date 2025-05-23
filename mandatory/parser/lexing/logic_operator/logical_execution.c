/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/23 22:57:40 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/**
 * @brief Executes a single AST node (command, pipe, etc.)
 * 
 * @param node The AST node to execute
 * @param env Environment structure
 * @return int Exit code of the execution
 */
static int	ft_execute_node(t_ast_node *node, t_env *env)
{
	t_command	*cmd;
	t_list		*cmd_list;
	int			exit_code;

	if (!node || !env)
		return (1);
	
	if (node->type == NODE_COMMAND)
	{
		cmd = (t_command *)node->data;
		if (!cmd || !cmd->args || !cmd->args[0])
			return (0);
		
		// Check if it's a builtin
		if (ft_is_builtin_cmd(cmd->args[0]))
			return (run_builtin(cmd->args, &env->env_vars));
		
		// Execute external command
		return (execute_command(cmd, env));
	}
	else if (node->type == NODE_PIPE)
	{
		// Convert AST back to command list for pipe execution
		cmd_list = ast_to_command_list(node);
		if (!cmd_list)
			return (1);
		
		exit_code = execute_pipe(cmd_list, env);
		ft_lstfree_cmd_list(cmd_list);
		return (exit_code);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
	{
		// This should be handled by ft_execute_logical
		return (ft_execute_logical(node, env));
	}
	
	return (1);
}

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
	
	// Execute left side first
	left_exit_code = ft_execute_logical(node->left, env);
	env->last_exit_code = left_exit_code;
	
	// Short-circuit evaluation
	if (node->type == NODE_AND)
	{
		// For &&: execute right only if left succeeded (exit code 0)
		if (left_exit_code == 0)
		{
			if (node->right)
			{
				right_exit_code = ft_execute_logical(node->right, env);
				env->last_exit_code = right_exit_code;
				return (right_exit_code);
			}
		}
		return (left_exit_code);
	}
	else if (node->type == NODE_OR)
	{
		// For ||: execute right only if left failed (exit code != 0)
		if (left_exit_code != 0)
		{
			if (node->right)
			{
				right_exit_code = ft_execute_logical(node->right, env);
				env->last_exit_code = right_exit_code;
				return (right_exit_code);
			}
		}
		return (left_exit_code);
	}
	
	return (1);
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
		return;
	
	for (i = 0; i < level; i++)
		ft_printf("  ");
	
	if (node->type == NODE_AND || node->type == NODE_OR || node->type == NODE_PIPE)
	{
		ft_printf("OPERATOR: %s\n", ft_get_logical_op_str(node->type));
		if (node->left)
		{
			for (i = 0; i < level + 1; i++)
				ft_printf("  ");
			ft_printf("LEFT:\n");
			ft_print_logical_ast(node->left, level + 2);
		}
		if (node->right)
		{
			for (i = 0; i < level + 1; i++)
				ft_printf("  ");
			ft_printf("RIGHT:\n");
			ft_print_logical_ast(node->right, level + 2);
		}
	}
	else if (node->type == NODE_COMMAND)
	{
		t_command *cmd = (t_command *)node->data;
		ft_printf("COMMAND: ");
		if (cmd && cmd->args && cmd->args[0])
		{
			for (i = 0; cmd->args[i]; i++)
			{
				ft_printf("%s", cmd->args[i]);
				if (cmd->args[i + 1])
					ft_printf(" ");
			}
		}
		ft_printf("\n");
	}
	else
	{
		ft_printf("NODE_TYPE: %d\n", node->type);
	}
}

/**
 * @brief Frees a command list
 * 
 * @param cmd_list List to free
 */
void	ft_lstfree_cmd_list(t_list *cmd_list)
{
	t_list		*current;
	t_list		*next;
	t_command	*cmd;
	int			i;

	if (!cmd_list)
		return;
	
	current = cmd_list;
	while (current)
	{
		next = current->next;
		
		if (current->type == TYPE_COMMAND && current->content.cmd)
		{
			cmd = current->content.cmd;
			if (cmd->args)
			{
				for (i = 0; cmd->args[i]; i++)
					free(cmd->args[i]);
				free(cmd->args);
			}
			// Note: Don't free cmd->redirs here as it's shared with AST
			free(cmd);
		}
		
		free(current);
		current = next;
	}
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
