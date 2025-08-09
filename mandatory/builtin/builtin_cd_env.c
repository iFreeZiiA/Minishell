/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:20:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/08 19:21:28 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	*get_env_value_shell(t_shell *shell, const char *key)
{
	char	**env;
	size_t	len;
	int		i;

	if (!shell || !shell->env || !shell->env->env_vars)
		return (NULL);
	env = shell->env->env_vars;
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!strncmp(env[i], key, len) && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*get_env_value_direct(char **env, const char *key)
{
	size_t	len;
	int		i;

	if (!env || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!strncmp(env[i], key, len) && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*get_cd_target(char **args, t_shell *shell)
{
	char	*target;

	target = NULL;
	if (!args[1])
	{
		target = get_env_value_shell(shell, "HOME");
		if (!target)
		{
			ft_printerr("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (!ft_strcmp(args[1], "-"))
	{
		target = get_env_value_shell(shell, "OLDPWD");
		if (!target)
		{
			ft_printerr("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		ft_printf("%s\n", target);
	}
	else
		target = args[1];
	return (target);
}

static char	*get_cd_target_direct(char **args, char ***env)
{
	char	*target;

	target = NULL;
	if (!args[1])
	{
		target = get_env_value_direct(*env, "HOME");
		if (!target)
		{
			ft_printerr("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (!ft_strcmp(args[1], "-"))
	{
		target = get_env_value_direct(*env, "OLDPWD");
		if (!target)
		{
			ft_printerr("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		ft_printf("%s\n", target);
	}
	else
		target = args[1];
	return (target);
}

static char	**alloc_env_with_new(char **env, char *new)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
	}
	new_env[i++] = new;
	new_env[i] = NULL;
	return (new_env);
}
