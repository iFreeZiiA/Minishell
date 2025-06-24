/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:04:08 by alearroy          #+#    #+#             */
/*   Updated: 2025/06/24 17:04:04 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	close_pipe_and_update(int *prev, int *pipe_fd)
{
	if (*prev != -1)
		close(*prev);
	*prev = pipe_fd[0];
	close(pipe_fd[1]);
}

void	wait_all_pids(pid_t *pids, int count, t_env *env)
{
	int		status;
	int		i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			env->last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			env->last_exit_code = 128 + WTERMSIG(status);
		i++;
	}
}

static void	exec_child_command(t_list *cmd_l, t_env *env)
{
	t_command	*cmd;
	char		*path;

	cmd = cmd_l->content.cmd;
	if (is_builtin(cmd->args[0]) && cmd_l->next == NULL)
		exit(run_builtin(cmd->args, env));
	path = get_path(cmd->args[0], env->env_vars);
	if (!path)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	execve(path, cmd->args, env->env_vars);
	ft_printerr(2, "minishell: %s: %s\n", cmd->args[0], strerror(errno));
	if (errno == EACCES)
		exit(126);
	exit(127);
}

void	child_process(t_list *cmd_l, int in, int out, t_env *env)
{
	t_command	*cmd;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cmd = cmd_l->content.cmd;
	if (in != -1)
		dup2(in, STDIN_FILENO);
	if (out != -1)
		dup2(out, STDOUT_FILENO);
	if (in != -1)
		close(in);
	if (out != -1)
		close(out);
	if (apply_redirections(cmd->redirs) != 0)
		exit(1);
	exec_child_command(cmd_l, env);
}