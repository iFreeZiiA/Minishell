/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:58:53 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 13:56:34 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	if (argc != 2)
	{
		ft_printerr("ERR ARG\n");
		return (1);
	}
	shell.current_line = argv[1];
	ft_lexing(&shell);
	shell.env->env_vars = env;
	ft_parsing(&shell);
	return (0);
}