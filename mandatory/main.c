/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:56:49 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/26 19:08:45 by alearroy         ###   ########.fr       */
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
	int		exit_code;
	int		parse_result;
	(void)argc;
	(void)argv;

	setup_interactive_signals();
	ft_setup(&shell, envp);
	while (1)
	{
		g_sig = 0; //RESET le signal a chaque nouvelle commande, a conserver
		input = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
		if (ft_exit(input) == -1)
			break;
		shell.current_line = input;
		shell.token = ft_lexing(&shell);
		add_history(input);
		parse_result = ft_parse(&shell); // Ne pas appelle 2 fois ft_parsing dans la meme boucle
		if (parse_result == 0 && shell.ast)
		{
			exit_code = executor_from_ast(shell.ast, shell.env);
			shell.env->last_exit_code = exit_code; // $?
		}
		else if (parse_result == -1)
			ft_printerr("WRONG ARG\n");
		free(input);
	}
	ft_cleanup(&shell, 0);
	return (0);
}
