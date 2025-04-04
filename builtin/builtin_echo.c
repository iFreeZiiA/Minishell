/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:47:21 by alearroy          #+#    #+#             */
/*   Updated: 2025/04/04 16:30:56 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_executor.h"

static int has_n_flag(char *arg)
{
	int i = 1;

	if (!arg || arg[0] != '-')
		return 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int builtin_echo(char **args)
{
	int i = 1;
	int newline = 1;

	while (args[i] && has_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

/* int main(int argc, char **argv)
{
	(void)argc;
	builtin_echo(**argv);
	return (0);
} */