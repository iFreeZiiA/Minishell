/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:56:49 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 16:23:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static inline int	ft_exit(char *input)
{
	if (!input || !ft_strcmp(input, "exit"))
	{
		if (input)
			free(input);
		write(1, "exit\n", 5);
		return (-1);
	}
	return (0);
}	

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	(void)argc;
	(void)argv;

	setup_interactive_signals();
	ft_setup(&shell, envp);
	while (1)
	{
		input = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
		if (ft_exit(input) == -1)
			break;
		shell.current_line = input;
		shell.token = ft_lexing(&shell);
		add_history(input);
		if (ft_parse(&shell) == -1)
			ft_printerr("WRONG ARG\n");
		//EXECUTOR
		free(input);
	}
	ft_cleanup(&shell, 0);
	return (0);
}
