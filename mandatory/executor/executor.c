/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:43:27 by alearroy          #+#    #+#             */
/*   Updated: 2025/06/12 18:52:36 by alearroy         ###   ########.fr       */
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
static int	open_and_dup(t_redir *r)
{
	int	fd;

	if (r->type == REDIR_IN)
		fd = open(r->file, O_RDONLY);
	else if (r->type == REDIR_OUT)
		fd = open(r->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (r->type == REDIR_APPEND)
		fd = open(r->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (r->type == REDIR_HEREDOC)
	{
		fd = handle_heredoc(r->file);
		if (fd == -1 || dup2(fd, STDIN_FILENO) < 0)
			return (-1);
		close(fd);
		return (0);
	}
	else
		return (-1);
	if (fd < 0)
		return (-1);
	if (dup2(fd, (r->type == REDIR_IN ? 0 : 1)) < 0)
		return (close(fd), -1);
	close(fd);
	return (0);
}

int	apply_redirections(t_list *redirs)
{
	t_redir	*r;

	while (redirs)
	{
		if (redirs->type != TYPE_REDIR)
			return (1);
		r = redirs->content.redir;
		if (open_and_dup(r) != 0)
			return (perror("minishell: redirection"), 1);
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
