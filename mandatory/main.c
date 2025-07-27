/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:56:49 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/27 15:44:57 by jjorda           ###   ########.fr       */
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
	printf("DEBUG: Environment setup completed\n");
	printf("DEBUG: Number of env vars: ");
	if (shell.env && shell.env->env_vars) {
		int count = 0;
		while (shell.env->env_vars[count]) count++;
		printf("%d\n", count);
	} else {
		printf("0 (env_vars is NULL)\n");
	}
	while (1)
	{
		g_sig = 0; //RESET le signal a chaque nouvelle commande, a conserver
		input = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
		if (ft_exit(input) == -1)
			break;
		shell.current_line = input;
		shell.token = ft_lexing(&shell);
		add_history(input);
		printf("DEBUG: Parsing command: %s\n", input);
		if (ft_parse_enhanced(&shell) == -1)
			ft_printerr("WRONG ARG\n");
		else
		{
			printf("DEBUG: Parse successful, executing simple command\n");
			// Executor simple pour test
			if (shell.token)
			{
				t_list *current = shell.token;
				while (current)
				{
					if (current->type == TYPE_TOKEN)
					{
						t_token *token = current->content.token;
						if (token && token->type == TOKEN_WORD)
						{
							if (!ft_strcmp(token->value, "env"))
							{
								printf("DEBUG: Executing env builtin\n");
								builtin_env(shell.env->env_vars);
							}
							else if (!ft_strcmp(token->value, "pwd"))
							{
								printf("DEBUG: Executing pwd builtin\n");
								builtin_pwd();
							}
							break;
						}
					}
					current = current->next;
				}
			}
		}
		//EXECUTOR
		free(input);
	}
	ft_cleanup(&shell, 0);
	return (0);
}
