/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:15:06 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static int	ft_init_shell_for_test(t_shell *shell, char **env)
{
	if (!shell)
		return (-1);
	shell->env = malloc(sizeof(t_env));
	if (!shell->env)
		return (-1);
	shell->env->env_vars = env;
	shell->env->local_env = NULL;
	shell->env->last_exit_code = 0;
	shell->token = NULL;
	shell->ast = NULL;
	shell->current_line = NULL;
	shell->prompt = NULL;
	return (0);
}

static void	ft_display_tokens(t_list *tokens)
{
	t_list	*curr;
	int		index;

	ft_printf("=== TOKEN LIST ===\n");
	if (!tokens)
	{
		ft_printf("(empty)\n\n");
		return ;
	}
	curr = tokens;
	index = 0;
	while (curr)
	{
		ft_printf("[%d] Type: %d, Value: \"%s\"\n", 
			index, curr->content.token->type, curr->content.token->value);
		curr = curr->next;
		index++;
	}
	ft_printf("\n");
}

static int	ft_test_parse_function(t_shell *shell)
{
	int	result;

	ft_printf("=== PARSING TEST ===\n");
	result = ft_parse(shell);
	if (result < 0)
	{
		ft_printf("✗ Parsing failed (code: %d)\n", result);
		return (1);
	}
	ft_printf("✓ Parsing successful\n\n");
	if (shell->ast)
	{
		ft_printf("=== AST STRUCTURE ===\n");
		ft_print_ast_tree(shell->ast, 0);
		ft_printf("\n");
	}
	else
		ft_printf("No AST generated\n\n");
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	int		exit_code;

	if (argc != 2)
	{
		ft_printf("Usage: %s \"command to parse\"\n", argv[0]);
		ft_printf("Example: %s \"echo hello | grep h && ls\"\n", argv[0]);
		return (1);
	}
	if (ft_init_shell_for_test(&shell, env) < 0)
	{
		ft_printf("Error: Shell initialization failed\n");
		return (1);
	}
	shell.current_line = argv[1];
	ft_printf("=== PARSING TEST PROGRAM ===\n");
	ft_printf("Command: \"%s\"\n\n", shell.current_line);
	shell.token = ft_lexing(&shell);
	if (!shell.token)
	{
		ft_printf("Error: Lexing failed\n");
		free(shell.env);
		return (1);
	}
	ft_display_tokens(shell.token);
	exit_code = ft_test_parse_function(&shell);
	if (shell.ast)
		ft_free_ast(shell.ast);
	if (shell.token)
		ft_lstfree_t(shell.token);
	free(shell.env);
	ft_printf("=== TEST COMPLETED ===\n");
	return (exit_code);
}
