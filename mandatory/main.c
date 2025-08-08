/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:56:49 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/08 20:50:50 by jjorda           ###   ########.fr       */
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
		
		// Protection contre les inputs trop longs pour éviter les segfaults
		if (ft_strlen(input) > 100000)
		{
			write(2, "minishell: command too long\n", 29);
			free(input);
			continue;
		}
		
		shell.current_line = input;
		if (ft_is_empty_ws(input))
		{
			free(input);
			continue;
		}
		int syntax_result = ft_validate_quotes_in_input(input);
		if (syntax_result != 0)
		{
			shell.last_exit_code = EXIT_SYNTAX;
			if (!is_interactive)
			{
				free(input);
				break;
			}
			free(input);
			continue;
		}
		
		// Validation contre les substitutions de commandes non supportées
		if (ft_validate_substitutions(input) != 0)
		{
			shell.last_exit_code = EXIT_SYNTAX;
			if (!is_interactive)
			{
				free(input);
				break;
			}
			free(input);
			continue;
		}
		add_history(input);
		int parse_result = ft_parse_input(input, &shell);
		if (parse_result == 0 && shell.ast)
		{
			int exec_result = executor_from_ast(shell.ast, shell.env);
			shell.last_exit_code = exec_result;
			ft_free_ast(shell.ast);
			shell.ast = NULL;
		}
		else
		{
			// En cas d'erreur de parsing, définir le code de sortie approprié
			if (parse_result == EXIT_SYNTAX)
				shell.last_exit_code = EXIT_SYNTAX;
			else
				shell.last_exit_code = EXIT_FAILURE;
				
			// En cas d'erreur, s'assurer que ast est NULL
			if (shell.ast)
			{
				ft_free_ast(shell.ast);
				shell.ast = NULL;
			}
			// L'erreur est déjà affichée par ft_parse_input via ft_val_syntax
			
			// En mode non-interactif, sortir immédiatement avec le code d'erreur
			if (!is_interactive)
			{
				free(input);
				break;
			}
		}
		free(input);
	}
	ft_cleanup(&shell, 0);
	return (shell.last_exit_code);
}
