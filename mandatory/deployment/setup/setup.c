/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:19:37 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/08 20:34:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static int	ft_allocate_env_vars(t_shell *shell, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	shell->env->env_vars = malloc(sizeof(char *) * (i + 1));
	if (!shell->env->env_vars)
	{
		free(shell->env);
		return (-1);
	}
	return (i);
}

static int	ft_copy_env_vars(t_shell *shell, char **env)
{
	int	i;

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
	return (0);
}

int	ft_setenv(t_shell *shell, char **env)
{
	int	count;

	if (!shell || !env)
		return (-1);
	shell->env = malloc(sizeof(t_env));
	if (!shell->env)
		return (-1);
	count = ft_allocate_env_vars(shell, env);
	if (count == -1)
		return (-1);
	if (ft_copy_env_vars(shell, env) == -1)
		return (-1);
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
	shell->last_exit_code = 0;
	return (0);
}
