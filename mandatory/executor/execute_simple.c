/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:15:32 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	ft_execute_builtin_parent(t_command *cmd, t_env *env)
{
	char	**env_backup;
	int		result;

	if (apply_redirections(cmd->redirs) != 0)
		return (1);
	ft_expand_command_args(cmd, env);
	env_backup = env->env_vars;
	result = run_builtin(cmd->args, &env->env_vars);
	if (env->env_vars != env_backup && env->env_vars != NULL)
	{
		env->last_exit_code = result;
	}
	return (result);
}

static int	ft_is_parent_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

static void	ft_execute_child_process(t_command *cmd, t_env *env)
{
	char	*cmd_path;

	setup_execution_signals();
	if (apply_redirections(cmd->redirs) != 0)
		exit(1);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd->args, &env->env_vars));
	cmd_path = get_path(cmd->args[0], env->env_vars);
	if (!cmd_path)
	{
		ft_printerr("minishell: command not found: ");
		ft_printerr(cmd->args[0]);
		ft_printerr("\n");
		exit(127);
	}
	execve(cmd_path, cmd->args, env->env_vars);
	free(cmd_path);
	ft_printerr("minishell: execve");
	exit(127);
}

int	execute_command(t_command *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->args[0]) && ft_is_parent_builtin(cmd->args[0]))
		return (ft_execute_builtin_parent(cmd, env));
	ft_expand_command_args(cmd, env);
	pid = fork();
	if (pid == -1)
		return (ft_printerr("minishell : fork"), 1);
	if (pid == 0)
		ft_execute_child_process(cmd, env);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	restore_interactive_signals();
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}
