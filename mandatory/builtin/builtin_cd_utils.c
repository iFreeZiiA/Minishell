/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:25:17 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 10:31:28 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	find_env_var(char **env, const char *key)
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
			if ((equal_pos - env[i]) == (int)key_len && 
				!strncmp(env[i], key, key_len))
				return (i);
		}
		i++;
	}
	return (-1);
}

int	update_env_var(char ***env, const char *key, const char *value)
{
	int		idx;
	char	*new;
	char	**new_env;
	int		env_size;

	if (!key || !value)
		return (1);
	new = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new)
		return (1);
	ft_strcpy(new, (char *)key);
	ft_strcat(new, "=");
	ft_strcat(new, value);
	
	// Utiliser notre nouvelle fonction de recherche
	idx = find_env_var(*env, key);
	if (idx >= 0)
	{
		free((*env)[idx]);
		(*env)[idx] = new;
		return (0);
	}
	
	// Compter la taille de l'environnement actuel
	env_size = 0;
	while ((*env)[env_size])
		env_size++;
	
	// Redimensionner l'environnement (au lieu d'en créer un nouveau)
	new_env = realloc(*env, sizeof(char *) * (env_size + 2));
	if (!new_env)
		return (free(new), 1);
	
	// Ajouter la nouvelle variable à la fin
	new_env[env_size] = new;
	new_env[env_size + 1] = NULL;
	
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
