/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:19:37 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/02 12:40:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

int	ft_setenv(t_shell *shell, char **env)
{
	int	i;

	if (!shell || !env)
		return (-1);
	shell->env = malloc(sizeof(t_env));
	if (!shell->env)
		return (-1);
	i = 0;
	while (env[i])
		i++;
	shell->env->env_vars = malloc(sizeof(char *) * (i + 1));
	if (!shell->env->env_vars)
	{
		free(shell->env);
		return (-1);
	}
	i = 0;
	while (env[i])
	{
		shell->env->env_vars[i] = ft_strdup(env[i]);
		if (!shell->env->env_vars[i])
		{
			while (--i >= 0)
				free(shell->env->env_vars[i]);
			free(shell->env->env_vars);
			free(shell->env);
			return (-1);
		}
		i++;
	}
	shell->env->env_vars[i] = NULL;
	shell->env->local_env = NULL;
	shell->env->last_exit_code = 0;
	return (0);
}

int	ft_setup(t_shell *shell, char **env)
{
	if (!shell || !env)
		return (-1);
	if (ft_setenv(shell, env) == -1)
		return (-1);
	shell->current_line = NULL;
	shell->signal_received = 0;
	shell->prompt = NULL;
	shell->ast = NULL;
	shell->token = NULL;
	return (0);
}
