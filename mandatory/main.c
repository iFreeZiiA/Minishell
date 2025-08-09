/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:56:49 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 10:31:28 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		is_interactive;
	int		result;

	(void)argc;
	(void)argv;
	setup_interactive_signals();
	ft_setup(&shell, envp);
	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		g_sig = 0;
		result = ft_handle_input(&shell, is_interactive);
		if (result == -1)
			break ;
	}
	ft_cleanup(&shell, 0);
	return (shell.last_exit_code);
}
