/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   .main.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:14:55 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/11 18:58:46 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	int		res;

	if (argc != 2)
	{
		ft_printerr("Please enter an argument to lexe...\n");
		return (0);
	}
	shell.current_line = argv[1];
	res = ft_lexing(&shell);
	if (res)
	{
		if (res == -1)
			ft_printerr("MALLOC ERRRO\n");
		if (res == -2)
			ft_printerr("WRITE ERROR\n");
		if (res == -3)
			ft_printerr("INVALID ARGUMENT\n");
		ft_printerr("%d\n", res);
		return (res);
	}
	shell.env->env_vars = env;
	ft_parsing(&shell);
	ft_print_list(shell.token);
	ft_lstfree_t(shell.token);
	return (0);
}
