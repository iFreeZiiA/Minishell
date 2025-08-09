/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:25:02 by jjorda            #+#    #+#             */
/*   Updated: 2025/01/17 18:25:02 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	ft_setup_child_fds(int in, int out)
{
	if (in != -1)
		dup2(in, STDIN_FILENO);
	if (out != -1)
		dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}

void	ft_execute_child_command(t_command *cmd, t_env *env)
{
	char	*cmd_path;

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
	perror("execve");
	exit(127);
}
