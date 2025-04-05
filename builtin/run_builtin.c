/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:38:09 by alearroy          #+#    #+#             */
/*   Updated: 2025/04/05 16:22:38 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int run_builtin(char **args)
{
	if (!ft_strcmp(args[1], "echo"))
		return (builtin_echo(args));
	if (!ft_strcmp(args[1], "pwd"))
		return (builtin_pwd());
	return (1);
}

int main(int argc, char **argv)
{
	(void)argc;
	run_builtin(argv);
	return (0);
}