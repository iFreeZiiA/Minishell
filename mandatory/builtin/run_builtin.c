/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:38:09 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 10:31:28 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	run_builtin(char **args, char ***env)
{
	int	result;

	if (!ft_strcmp(args[0], "echo"))
		return (builtin_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (builtin_pwd());
	if (!ft_strcmp(args[0], "cd"))
	{
		// Créer une structure shell temporaire pour l'interface
		t_shell shell;
		t_env env_struct;
		
		env_struct.env_vars = *env;
		shell.env = &env_struct;
		result = builtin_cd(args, &shell);
		*env = shell.env->env_vars;  // Propagation des changements
		return (result);
	}
	if (!ft_strcmp(args[0], "env"))
		return (builtin_env(*env));
	if (!ft_strcmp(args[0], "unset"))
	{
		result = builtin_unset(args, env);
		return (result);
	}
	if (!ft_strcmp(args[0], "exit"))
		return (builtin_exit(args));
	if (!ft_strcmp(args[0], "export"))
	{
		result = builtin_export(args, env);
		return (result);
	}
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
/* int	main(int argc, char **argv, char **envp)
{
	char	**env;
	(void)argc;
	env = ft_envdup(envp);
	run_builtin(argv, &env);
	return (0);
} */
