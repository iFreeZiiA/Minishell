/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:55:58 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Tests logical operator detection
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_logical_detection(void)
{
	t_token	and_token;
	t_token	or_token;
	t_token	word_token;

	and_token.type = TOKEN_AND;
	or_token.type = TOKEN_OR;
	word_token.type = TOKEN_WORD;
	if (!ft_is_logical_operator_token(&and_token))
		return (-1);
	if (!ft_is_logical_operator_token(&or_token))
		return (-1);
	if (ft_is_logical_operator_token(&word_token))
		return (-1);
	return (0);
}

/**
 * @brief Tests operator precedence
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_precedence(void)
{
	int	pipe_prec;
	int	and_prec;
	int	or_prec;

	pipe_prec = ft_get_operator_precedence(TOKEN_PIPE);
	and_prec = ft_get_operator_precedence(TOKEN_AND);
	or_prec = ft_get_operator_precedence(TOKEN_OR);
	if (pipe_prec <= and_prec)
		return (-1);
	if (and_prec <= or_prec)
		return (-1);
	return (0);
}

/**
 * @brief Tests complete logical parsing
 * 
 * @param test_cmd Command to test
 * @param env Environment
 * @return int 0 on success, -1 on error
 */
static int	ft_test_parsing(char *test_cmd, char **env)
{
	t_shell		shell;
	t_ast_node	*ast;

	if (ft_setup(&shell, env) == -1)
		return (-1);
	shell.current_line = test_cmd;
	shell.token = ft_lexing(&shell);
	if (!shell.token)
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	if (ft_parse_logical_operators(&shell) == -1)
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	ast = shell.ast;
	if (!ft_validate_logical_ast(ast))
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	ft_cleanup(&shell, 0);
	return (0);
}

/**
 * @brief Main test function
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @param env Environment
 * @return int Exit code
 */
int	main(int argc, char **argv, char **env)
{
	int	test1;
	int	test2;
	int	test3;

	(void)argc;
	(void)argv;
	ft_printf("=== Tests Logical Operators Phase 7.1 ===\n");
	test1 = ft_test_logical_detection();
	ft_printf("Test Detection: %s\n", test1 == 0 ? "✓ PASS" : "✗ FAIL");
	test2 = ft_test_precedence();
	ft_printf("Test Precedence: %s\n", test2 == 0 ? "✓ PASS" : "✗ FAIL");
	test3 = ft_test_parsing("echo hello && echo world", env);
	ft_printf("Test Parsing: %s\n", test3 == 0 ? "✓ PASS" : "✗ FAIL");
	if (test1 == 0 && test2 == 0 && test3 == 0)
	{
		ft_printf("✅ Tous les tests Phase 7.1 sont passés !\n");
		return (0);
	}
	ft_printf("❌ Certains tests ont échoué.\n");
	return (1);
}
