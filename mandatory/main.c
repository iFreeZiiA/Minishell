/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:56:49 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 22:39:22 by jjorda           ###   ########.fr       */
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
	int		is_interactive;

	(void)argc;
	(void)argv;
	setup_interactive_signals();
	ft_setup(&shell, envp);
	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		g_sig = 0;
		if (is_interactive)
			input = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
		else
		{
			size_t len = 0;
			input = NULL;
			if (getline(&input, &len, stdin) == -1)
			{
				free(input);
				break;
			}
			// Enlever le newline à la fin
			len = ft_strlen(input);
			if (len > 0 && input[len - 1] == '\n')
				input[len - 1] = '\0';
		}
		if (ft_exit(input) == -1)
			break;
		shell.current_line = input;
		if (ft_is_empty_ws(input))
		{
			free(input);
			continue;
		}
		int syntax_result = ft_validate_quotes_in_input(input);
		if (syntax_result != 0)
		{
			free(input);
			continue;
		}
		add_history(input);
		int parse_result = ft_parse_input(input, &shell);
		if (parse_result == 0 && shell.ast)
		{
			executor_from_ast(shell.ast, shell.env);
		}
		else
		{
			ft_printerr("bash: syntax error in command line\n");
		}
		free(input);
	}
	ft_cleanup(&shell, 0);
	return (0);
}
