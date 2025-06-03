/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:43:27 by alearroy          #+#    #+#             */
/*   Updated: 2025/06/03 13:55:00 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (
		!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit")
	);
}

static int	apply_redirections(t_list *redirs)
{
	t_redir	*r;
	int		fd;

	while (redirs)
	{
		if (redirs->type != TYPE_REDIR)
			return (1);
		r = redirs->content.redir;
		if (r->type == REDIR_IN)
			fd = open(r->file, O_RDONLY);
		else if (r->type == REDIR_OUT)
			fd = open(r->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (r->type == REDIR_APPEND)
			fd = open(r->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			return (1); // Pas ici qu'on fait les heredocs;'\]
		if (fd < 0 || dup2(fd,
			(r->type == REDIR_IN ? STDIN_FILENO : STDOUT_FILENO)) < 0)
			return (perror("minishell: redir"), 1);
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}

int	execute_command(t_command *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid = -1)
		return (ft_printerr("minishell : fork"),1);
	if (pid == 0)
	{
		if (apply_redirections(cmd->redirs) != 0)
			exit (1);
		if (is_builtin(cmd->args[0]))
			exit(run_builtin(cmd->args, env));
		execve(get_path(cmd->args[0], env->env_vars),
			cmd->args, env->env_vars);
		ft_printerr("minishell: execve");
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		env->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		env->last_exit_code = 128 + WTERMSIG(status);
	return (env->last_exit_code);
}

static void	close_pipe_and_update(int *prev, int *pipe_fd)
{
	if (*prev != -1)
		close(*prev);
	*prev = pipe_fd[0];
	close(pipe_fd[1]);
}

static void	wait_all_pids(pid_t *pids, int count, t_env *env)
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

static void	child_process(t_command *cmd, int in, int out, t_env *env)
{
	if (in != -1)
		dup2(in, STDIN_FILENO);
	if (out != -1)
		dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
	if (apply_redirections(cmd->redirs) != 0)
		exit(1);
	if (is_builtin(cmd->args[0]) && !cmd->next)
		exit(run_builtin(cmd->args, env));
	execve(get_path(cmd->args[0], env->env_vars),
		cmd->args, env->env_vars);
	perror("execve");
	exit(127);
}

int	execute_pipe(t_list *cmd_h, t_env *env)
{
	pid_t	*pids;
	int		pipe_fd[2];
	int		prev;
	int		i;

	i = 0;
	prev = -1;
	pids = malloc(sizeof(pid_t) * ft_lstsize(cmd_h));
	if (!pids)
		return (1);
	while (cmd_h)
	{
		if (cmd_h->next && pipe(pipe_fd) == -1)
			return (free(pids), 1);
		pids[i] = fork();
		if (pids[i] == 0)
			child_process(cmd_h, prev, (cmd_h->next ? pipe_fd[1] : -1), env);
		if (prev != -1)
			close(prev);
		if (cmd_h->next)
			close_pipe_and_update(&prev, pipe_fd);
		cmd_h = cmd_h->next;
		i++;
	}
	wait_all_pids(pids, i, env);
	return (free(pids), 0);
}

void test(int test)
{
	
}