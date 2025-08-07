/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:25:17 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/07 22:39:22 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

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

static void	ft_free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

int	update_env_var(char ***env, const char *key, const char *value)
{
	int		idx;
	char	*new;
	char	**new_env;

	if (!key || !value)
		return (1);
	new = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new)
		return (1);
	ft_strcpy(new, (char *)key);
	ft_strcat(new, "=");
	ft_strcat(new, value);
	idx = get_env_index(*env, key);
	if (idx >= 0)
	{
		free((*env)[idx]);
		(*env)[idx] = new;
		return (0);
	}
	new_env = alloc_env_with_new(*env, new);
	if (!new_env)
		return (free(new), 1);
	ft_free_env(*env);
	*env = new_env;
	return (0);
}

int	get_env_index(char **env, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
