/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/23 23:02:07 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/**
 * @brief Enhanced logical execution with parentheses support
 * 
 * @param node AST node containing the expression
 * @param env Environment structure
 * @return int Final exit code
 */
int	ft_execute_logical_enhanced(t_ast_node *node, t_env *env)
{
	int	left_exit_code;
	int	right_exit_code;

	if (!node || !env)
		return (1);
	
	// Handle different node types
	switch (node->type)
	{
		case NODE_GROUP:
			return (ft_execute_group(node, env));
		
		case NODE_COMMAND:
			return (ft_execute_single_command(node, env));
		
		case NODE_PIPE:
			return (ft_execute_pipe_from_ast(node, env));
		
		case NODE_AND:
		case NODE_OR:
			// Execute left side first
			if (!node->left)
				return (1);
			
			left_exit_code = ft_execute_logical_enhanced(node->left, env);
			env->last_exit_code = left_exit_code;
			
			// Short-circuit evaluation
			if (node->type == NODE_AND)
			{
				// For &&: execute right only if left succeeded (exit code 0)
				if (left_exit_code == 0 && node->right)
				{
					right_exit_code = ft_execute_logical_enhanced(node->right, env);
					env->last_exit_code = right_exit_code;
					return (right_exit_code);
				}
				return (left_exit_code);
			}
			else // NODE_OR
			{
				// For ||: execute right only if left failed (exit code != 0)
				if (left_exit_code != 0 && node->right)
				{
					right_exit_code = ft_execute_logical_enhanced(node->right, env);
					env->last_exit_code = right_exit_code;
					return (right_exit_code);
				}
				return (left_exit_code);
			}
		
		default:
			ft_printerr("minishell: unknown node type: %d\n", node->type);
			return (1);
	}
}

/**
 * @brief Executes a single command node
 * 
 * @param node Command node
 * @param env Environment
 * @return int Exit code
 */
int	ft_execute_single_command(t_ast_node *node, t_env *env)
{
	t_command	*cmd;

	if (!node || !env || node->type != NODE_COMMAND)
		return (1);
	
	cmd = (t_command *)node->data;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
	// Check if it's a builtin
	if (ft_is_builtin_cmd(cmd->args[0]))
		return (run_builtin(cmd->args, &env->env_vars));
	
	// Execute external command
	return (execute_command(cmd, env));
}

/**
 * @brief Executes a pipe directly from AST
 * 
 * @param node Pipe node
 * @param env Environment
 * @return int Exit code
 */
int	ft_execute_pipe_from_ast(t_ast_node *node, t_env *env)
{
	t_list	*cmd_list;
	int		exit_code;

	if (!node || !env || node->type != NODE_PIPE)
		return (1);
	
	// Convert AST back to command list for pipe execution
	cmd_list = ast_to_command_list(node);
	if (!cmd_list)
		return (1);
	
	exit_code = execute_pipe(cmd_list, env);
	ft_lstfree_cmd_list(cmd_list);
	
	return (exit_code);
}

/**
 * @brief Enhanced pipe execution with better error handling
 * 
 * @param cmd_h Command list head
 * @param env Environment
 * @return int Exit code of last command in pipe
 */
int	execute_pipe(t_list *cmd_h, t_env *env)
{
	int		num_cmds;
	int		*pipe_fds;
	pid_t	*pids;
	int		i;
	int		status;
	int		exit_code;

	if (!cmd_h || !env)
		return (1);
	
	num_cmds = ft_lstsize(cmd_h);
	if (num_cmds == 1)
	{
		// Single command, no pipe needed
		if (cmd_h->type == TYPE_COMMAND)
			return (ft_execute_single_command_from_list(cmd_h, env));
		return (1);
	}
	
	// Allocate pipe file descriptors
	pipe_fds = (int *)malloc(sizeof(int) * (num_cmds - 1) * 2);
	pids = (pid_t *)malloc(sizeof(pid_t) * num_cmds);
	
	if (!pipe_fds || !pids)
	{
		free(pipe_fds);
		free(pids);
		return (1);
	}
	
	// Create all pipes
	for (i = 0; i < num_cmds - 1; i++)
	{
		if (pipe(&pipe_fds[i * 2]) == -1)
		{
			ft_printerr("minishell: pipe failed\n");
			free(pipe_fds);
			free(pids);
			return (1);
		}
	}
	
	// Execute each command in the pipe
	exit_code = ft_execute_pipe_commands(cmd_h, env, pipe_fds, pids, num_cmds);
	
	// Close all pipe file descriptors in parent
	for (i = 0; i < (num_cmds - 1) * 2; i++)
		close(pipe_fds[i]);
	
	// Wait for all children and get exit code of last command
	for (i = 0; i < num_cmds; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_cmds - 1) // Last command determines exit code
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
	}
	
	free(pipe_fds);
	free(pids);
	
	env->last_exit_code = exit_code;
	return (exit_code);
}

/**
 * @brief Executes commands in a pipe sequence
 * 
 * @param cmd_h Command list head
 * @param env Environment
 * @param pipe_fds Array of pipe file descriptors
 * @param pids Array to store process IDs
 * @param num_cmds Number of commands
 * @return int Exit code
 */
static int	ft_execute_pipe_commands(t_list *cmd_h, t_env *env, int *pipe_fds,
                                     pid_t *pids, int num_cmds)
{
	t_list		*curr;
	t_command	*cmd;
	int			i;
	char		*path;

	curr = cmd_h;
	i = 0;
	
	while (curr && i < num_cmds)
	{
		if (curr->type != TYPE_COMMAND)
		{
			curr = curr->next;
			continue;
		}
		
		cmd = curr->content.cmd;
		pids[i] = fork();
		
		if (pids[i] == -1)
		{
			ft_printerr("minishell: fork failed\n");
			return (1);
		}
		
		if (pids[i] == 0)
		{
			// Child process
			ft_setup_pipe_redirections(i, num_cmds, pipe_fds);
			
			// Apply command-specific redirections
			if (cmd->redirs && apply_redirections(cmd->redirs) != 0)
				exit(1);
			
			if (ft_is_builtin_cmd(cmd->args[0]))
				exit(run_builtin(cmd->args, &env->env_vars));
			
			path = get_path(cmd->args[0], env->env_vars);
			if (!path)
			{
				ft_printerr("minishell: command not found: %s\n", cmd->args[0]);
				exit(127);
			}
			
			execve(path, cmd->args, env->env_vars);
			ft_printerr("minishell: execve failed\n");
			free(path);
			exit(127);
		}
		
		curr = curr->next;
		i++;
	}
	
	return (0);
}

/**
 * @brief Sets up pipe redirections for a command in a pipe sequence
 * 
 * @param cmd_index Command index in the pipe
 * @param num_cmds Total number of commands
 * @param pipe_fds Array of pipe file descriptors
 */
static void	ft_setup_pipe_redirections(int cmd_index, int num_cmds, int *pipe_fds)
{
	int	i;

	// Redirect input from previous pipe (except for first command)
	if (cmd_index > 0)
	{
		if (dup2(pipe_fds[(cmd_index - 1) * 2], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 input");
			exit(1);
		}
	}
	
	// Redirect output to next pipe (except for last command)
	if (cmd_index < num_cmds - 1)
	{
		if (dup2(pipe_fds[cmd_index * 2 + 1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 output");
			exit(1);
		}
	}
	
	// Close all pipe file descriptors in child
	for (i = 0; i < (num_cmds - 1) * 2; i++)
		close(pipe_fds[i]);
}

/**
 * @brief Executes a single command from a command list
 * 
 * @param cmd_node Command node
 * @param env Environment
 * @return int Exit code
 */
static int	ft_execute_single_command_from_list(t_list *cmd_node, t_env *env)
{
	t_command	*cmd;

	if (!cmd_node || cmd_node->type != TYPE_COMMAND || !env)
		return (1);
	
	cmd = cmd_node->content.cmd;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
	// Check if it's a builtin
	if (ft_is_builtin_cmd(cmd->args[0]))
		return (run_builtin(cmd->args, &env->env_vars));
	
	// Execute external command with redirections
	return (execute_command(cmd, env));
}

/**
 * @brief Main enhanced execution function with full parentheses support
 * 
 * @param shell Shell structure
 * @return int Exit code of the execution
 */
int	ft_execute_ast_enhanced(t_shell *shell)
{
	int	exit_code;

	if (!shell || !shell->ast || !shell->env)
		return (1);
	
	exit_code = ft_execute_logical_enhanced(shell->ast, shell->env);
	shell->env->last_exit_code = exit_code;
	
	return (exit_code);
}
