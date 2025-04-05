/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:48:15 by alearroy          #+#    #+#             */
/*   Updated: 2025/04/05 18:25:45 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

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
	if (!args[2])
		target = get_env_value(*env, "HOME");
	else if (!ft_strcmp(args[2], "-"))
	{
		target = get_env_value(*env, "OLDPWD");
		if (target)
			ft_printf("%s\n", target);
	}
	else
		target = args[2];
	return (target);
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
	chdir(target);
	if (target != 0)
	{
		ft_printerr("minishell: cd");
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
