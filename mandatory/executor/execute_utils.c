/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:15:52 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static void	ft_create_temp_shell(t_shell *temp_shell, t_env *env)
{
	temp_shell->env = env;
}

void	ft_expand_command_args(t_command *cmd, t_env *env)
{
	t_shell	temp_shell;
	int		i;

	if (!cmd || !cmd->args || !env)
		return ;
	ft_create_temp_shell(&temp_shell, env);
	i = 0;
	while (cmd->args[i])
	{
		if (ft_should_skip_expansion(cmd->args[0], i, cmd->args[i]))
			ft_process_special_arg(&cmd->args[i]);
		else
			ft_process_normal_arg(&cmd->args[i], env->env_vars, &temp_shell);
		i++;
	}
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}
