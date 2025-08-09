/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:25:17 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 10:59:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	find_env_var(char **env, const char *key)
{
	int		i;
	size_t	key_len;
	char	*equal_pos;

	if (!env || !key)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		equal_pos = ft_strchr(env[i], '=');
		if (equal_pos)
		{
			if ((equal_pos - env[i]) == (int)key_len
				&& !strncmp(env[i], key, key_len))
				return (i);
		}
		i++;
	}
	return (-1);
}

static char	*create_env_string(const char *key, const char *value)
{
	char	*new;

	new = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new)
		return (NULL);
	ft_strcpy(new, (char *)key);
	ft_strcat(new, "=");
	ft_strcat(new, value);
	return (new);
}

static int	replace_existing_var(char ***env, int idx, char *new)
{
	free((*env)[idx]);
	(*env)[idx] = new;
	return (0);
}

static int	add_new_var(char ***env, char *new)
{
	char	**new_env;
	int		env_size;

	env_size = 0;
	while ((*env)[env_size])
		env_size++;
	new_env = realloc(*env, sizeof(char *) * (env_size + 2));
	if (!new_env)
		return (free(new), 1);
	new_env[env_size] = new;
	new_env[env_size + 1] = NULL;
	*env = new_env;
	return (0);
}

int	update_env_var(char ***env, const char *key, const char *value)
{
	int		idx;
	char	*new;

	if (!key || !value)
		return (1);
	new = create_env_string(key, value);
	if (!new)
		return (1);
	idx = find_env_var(*env, key);
	if (idx >= 0)
		return (replace_existing_var(env, idx, new));
	return (add_new_var(env, new));
}
