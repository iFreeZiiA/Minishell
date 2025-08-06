/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:48:15 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/02 12:40:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	*get_env_value(char **env, const char *key)
{
	size_t	len;
	int		i;

	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}
static char	*get_cd_target(char **args, char ***env)
{
	char	*target;

	target = NULL;
	if (!args[1])
		target = get_env_value(*env, "HOME");
	else if (!ft_strcmp(args[1], "-"))
	{
		target = get_env_value(*env, "OLDPWD");
		if (target)
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

int	builtin_cd(char **args, char ***env)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	target = get_cd_target(args, env);
	if (!target)
	{
		ft_printerr("minishell: cd: target not set\n");
		free(oldpwd);
		return (1);
	}
	if (chdir(target) != 0)
	{
		ft_printerr("minishell: cd\n");
		free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	update_env_var(env, "OLDPWD", oldpwd);
	update_env_var(env, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}
