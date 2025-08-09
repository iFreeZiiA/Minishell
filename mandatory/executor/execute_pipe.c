/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 15:00:00 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	ft_handle_single_builtin(t_list *cmd_h, t_env *env)
{
	t_command	*cmd;
	char		**env_backup;
	int			result;

	cmd = cmd_h->content.cmd;
	if (apply_redirections(cmd->redirs) != 0)
		return (1);
	env_backup = env->env_vars;
	result = run_builtin(cmd->args, &env->env_vars);
	if (env->env_vars != env_backup && env->env_vars != NULL)
	{
		env->last_exit_code = result;
	}
	return (result);
}

static int	ft_is_single_parent_builtin(t_list *cmd_h)
{
	t_command	*cmd;

	if (!cmd_h || cmd_h->next)
		return (0);
	cmd = cmd_h->content.cmd;
	if (!is_builtin(cmd->args[0]))
		return (0);
	if (!ft_strcmp(cmd->args[0], "cd") || !ft_strcmp(cmd->args[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "unset") || !ft_strcmp(cmd->args[0], "exit"))
		return (1);
	return (0);
}

void	child_process(t_list *cmd_l, int in, int out, t_env *env)
{
	t_command	*cmd;

	cmd = cmd_l->content.cmd;
	ft_expand_command_args(cmd, env);
	ft_setup_child_fds(in, out);
	if (apply_redirections(cmd->redirs) != 0)
		exit(1);
	if (is_builtin(cmd->args[0]) && !cmd_l->next)
		exit(run_builtin(cmd->args, &env->env_vars));
	ft_execute_child_command(cmd, env);
}

int	execute_pipe(t_list *cmd_h, t_env *env)
{
	pid_t	*pids;
	int		count;

	if (ft_is_single_parent_builtin(cmd_h))
		return (ft_handle_single_builtin(cmd_h, env));
	if (cmd_h && !cmd_h->next)
		ft_expand_command_args(cmd_h->content.cmd, env);
	pids = malloc(sizeof(pid_t) * ft_lstsize(cmd_h));
	if (!pids)
		return (1);
	count = ft_execute_pipe_loop(cmd_h, pids, env);
	if (count == -1)
		return (free(pids), 1);
	wait_all_pids(pids, count, env);
	free(pids);
	return (env->last_exit_code);
}
