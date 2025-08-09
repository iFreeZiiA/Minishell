/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:38:09 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 10:59:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	ft_run_cd_builtin(char **args, char ***env)
{
	t_shell	shell;
	t_env	env_struct;
	int		result;

	env_struct.env_vars = *env;
	shell.env = &env_struct;
	result = builtin_cd(args, &shell);
	*env = shell.env->env_vars;
	return (result);
}

static int	ft_run_unset_builtin(char **args, char ***env)
{
	int	result;

	result = builtin_unset(args, env);
	return (result);
}

static int	ft_run_export_builtin(char **args, char ***env)
{
	int	result;

	result = builtin_export(args, env);
	return (result);
}

int	run_builtin(char **args, char ***env)
{
	if (!ft_strcmp(args[0], "echo"))
		return (builtin_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (builtin_pwd());
	if (!ft_strcmp(args[0], "cd"))
		return (ft_run_cd_builtin(args, env));
	if (!ft_strcmp(args[0], "env"))
		return (builtin_env(*env));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_run_unset_builtin(args, env));
	if (!ft_strcmp(args[0], "exit"))
		return (builtin_exit(args));
	if (!ft_strcmp(args[0], "export"))
		return (ft_run_export_builtin(args, env));
	return (1);
}

char	**ft_envdup(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = -1;
	while (envp[++i])
		copy[i] = ft_strdup(envp[i]);
	copy[i] = NULL;
	return (copy);
}
