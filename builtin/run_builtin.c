/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:38:09 by alearroy          #+#    #+#             */
/*   Updated: 2025/04/05 18:17:52 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	run_builtin(char **args, char **env)
{
	if (!ft_strcmp(args[1], "echo"))
		return (builtin_echo(args));
	if (!ft_strcmp(args[1], "pwd"))
		return (builtin_pwd());
	if (!ft_strcmp(args[0], "cd"))
		return (builtin_cd(args, &env));
	return (1);
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	run_builtin(argv, env);
	return (0);
}