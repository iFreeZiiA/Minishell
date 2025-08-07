/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:48:15 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/07 22:39:22 by jjorda           ###   ########.fr       */
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

// Version directe qui utilise char **env au lieu de t_shell
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

// Version directe pour éviter les problèmes de wrapper
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

int	update_env_var_shell(t_shell *shell, const char *key, const char *value)
{
	int		idx;
	char	*new;
	char	**new_env;

	if (!key || !value || !shell || !shell->env)
		return (1);
	new = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!new)
		return (1);
	ft_strcpy(new, (char *)key);
	ft_strcat(new, "=");
	ft_strcat(new, value);
	idx = get_env_index(shell->env->env_vars, key);
	if (idx >= 0)
	{
		free(shell->env->env_vars[idx]);
		shell->env->env_vars[idx] = new;
		return (0);
	}
	new_env = alloc_env_with_new(shell->env->env_vars, new);
	if (!new_env)
		return (free(new), 1);
	ft_free_env(shell->env->env_vars);
	shell->env->env_vars = new_env;
	return (0);
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
	update_env_var_shell(shell, "OLDPWD", oldpwd);
	update_env_var_shell(shell, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}

// Version simplifiée qui utilise directement char ***env pour éviter les problèmes mémoire
int	builtin_cd_direct(char **args, char ***env)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	target = get_cd_target_direct(args, env);
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
	update_env_var(env, "OLDPWD", oldpwd);
	update_env_var(env, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}
