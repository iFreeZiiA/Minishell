/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:53:36 by alearroy          #+#    #+#             */
/*   Updated: 2025/06/24 18:53:36 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	exec_pipe_node(t_list *node, t_env *env,
				pid_t *pids, int *prev, int index)
{
	int			pipe_fd[2];
	t_command	*cmd;

	cmd = node->content.cmd;
	if (node->next && pipe(pipe_fd) == -1)
		return (1);
	pids[index] = fork();
	if (pids[index] == 0)
	{
		if (node->next)
			child_process(node, *prev, pipe_fd[1], env);
		else
			child_process(node, *prev, -1, env);
	}
	if (*prev != -1)
		close(*prev);
	if (node->next)
		close_pipe_and_update(prev, pipe_fd);
	return (0);
}

static int	exec_pipe_loop(t_list *cmds, t_env *env,
				pid_t *pids, int *prev, int *i)
{
	while (cmds)
	{
		if (exec_pipe_node(cmds, env, pids, prev, *i))
			return (1);
		cmds = cmds->next;
		(*i)++;
	}
	return (0);
}

static int	must_run_in_parent(char *cmd)
{
	return (
		!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
	);
}

static int	execute_single_command(t_list *cmd_h, t_env *env)
{
	t_command	*cmd;

	cmd = cmd_h->content.cmd;
	if (is_builtin(cmd->args[0]) && must_run_in_parent(cmd->args[0]))
	{
		if (apply_redirections(cmd->redirs) != 0)
			return (1);
		return (run_builtin(cmd->args, &(env->env_vars)));
	}
	return (execute_command(cmd, env));
}

int	execute_pipe(t_list *cmd_h, t_env *env)
{
	pid_t	*pids;
	int		i;
	int		prev;

	if (!cmd_h)
		return (1);
	if (!cmd_h->next)
		return execute_single_command(cmd_h, env);

	i = 0;
	prev = -1;
	pids = malloc(sizeof(pid_t) * ft_lstsize(cmd_h));
	if (!pids)
		return (1);
	if (exec_pipe_loop(cmd_h, env, pids, &prev, &i))
	{
		free(pids);
		return (1);
	}
	wait_all_pids(pids, i, env);
	free(pids);
	return (0);
}

