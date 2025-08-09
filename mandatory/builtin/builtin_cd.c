/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:48:15 by alearroy          #+#    #+#             */
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

static int	ft_update_shell_env(t_shell *shell, const char *key,
		const char *value)
{
	return (update_env_var(&(shell->env->env_vars), key, value));
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	target = get_cd_target(args, shell);
	if (!target)
	{
		free(oldpwd);
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	ft_update_shell_env(shell, "OLDPWD", oldpwd);
	ft_update_shell_env(shell, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}
