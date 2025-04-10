/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:43:27 by alearroy          #+#    #+#             */
/*   Updated: 2025/04/10 18:49:46 by alearroy         ###   ########.fr       */
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
			return (1); // Pas ici qu'on fait les heredocs
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
	{
		ft_printerr("minishell : fork");
		return (1);
	}
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
	env->last_exit_code = WEXITSTATUS(status);
	return (env->last_exit_code);
}
