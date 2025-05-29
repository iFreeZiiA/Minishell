/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_execution_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:40:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/29 18:15:50 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Handles OR operator logic with short-circuit evaluation
 * 
 * @param node AST node
 * @param env Environment
 * @param left_exit_code Exit code from left side
 * @return int Final exit code
 */
int	ft_handle_or_operator(t_ast_node *node, t_env *env, int left_exit_code)
{
	int	right_exit_code;

	if (left_exit_code != 0 && node->right)
	{
		right_exit_code = ft_execute_logical(node->right, env);
		env->last_exit_code = right_exit_code;
		return (right_exit_code);
	}
	return (left_exit_code);
}

/**
 * @brief Executes a single AST node (command, pipe, etc.)
 * 
 * @param node The AST node to execute
 * @param env Environment structure
 * @return int Exit code of the execution
 */
int	ft_execute_node(t_ast_node *node, t_env *env)
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
		if (ft_is_builtin_cmd(cmd->args[0]))
			return (run_builtin(cmd->args, &env->env_vars));
		return (execute_command(cmd, env));
	}
	if (node->type == NODE_PIPE)
		return (ft_execute_pipe_node(node, env));
	if (node->type == NODE_GROUP)
		return (ft_execute_subshell(node->left, env));
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (ft_execute_logical(node, env));
	return (1);
}

/**
 * @brief Executes a pipe node
 * 
 * @param node Pipe AST node
 * @param env Environment
 * @return int Exit code
 */
int	ft_execute_pipe_node(t_ast_node *node, t_env *env)
{
	t_list	*cmd_list;
	int		exit_code;

	cmd_list = ast_to_command_list(node);
	if (!cmd_list)
		return (1);
	exit_code = execute_pipe(cmd_list, env);
	ft_lstfree_cmd_list(cmd_list);
	return (exit_code);
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

	if (!cmd_list)
		return ;
	current = cmd_list;
	while (current)
	{
		next = current->next;
		if (current->type == TYPE_COMMAND && current->content.cmd)
			ft_free_command_from_list(current->content.cmd);
		free(current);
		current = next;
	}
}

/**
 * @brief Frees a command structure from list
 * 
 * @param cmd Command to free
 */
void	ft_free_command_from_list(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}
