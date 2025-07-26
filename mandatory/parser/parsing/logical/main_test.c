/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 21:12:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Tests basic logical operator detection
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_basic_detection(void)
{
	t_token	and_token;
	t_token	or_token;
	t_token	pipe_token;
	t_token	word_token;

	and_token.type = TOKEN_AND;
	or_token.type = TOKEN_OR;
	pipe_token.type = TOKEN_PIPE;
	word_token.type = TOKEN_WORD;
	if (!ft_is_logical_operator_token(&and_token))
		return (-1);
	if (!ft_is_logical_operator_token(&or_token))
		return (-1);
	if (ft_is_logical_operator_token(&pipe_token))
		return (-1);
	if (ft_is_logical_operator_token(&word_token))
		return (-1);
	return (0);
}

/**
 * @brief Tests operator precedence values
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_precedence_values(void)
{
	int	pipe_prec;
	int	and_prec;
	int	or_prec;
	int	unknown_prec;

	pipe_prec = ft_get_operator_precedence(TOKEN_PIPE);
	and_prec = ft_get_operator_precedence(TOKEN_AND);
	or_prec = ft_get_operator_precedence(TOKEN_OR);
	unknown_prec = ft_get_operator_precedence(TOKEN_WORD);
	if (pipe_prec != 3)
		return (-1);
	if (and_prec != 2)
		return (-1);
	if (or_prec != 1)
		return (-1);
	if (unknown_prec != 0)
		return (-1);
	return (0);
}

/**
 * @brief Tests simple logical command parsing
 * 
 * @param test_cmd Command to test
 * @param env Environment variables
 * @return int 0 on success, -1 on error
 */
static int	ft_test_simple_logical(char *test_cmd, char **env)
{
	t_shell		shell;
	t_ast_node	*ast;
	int			result;

	if (ft_setup(&shell, env) == -1)
		return (-1);
	shell.current_line = test_cmd;
	shell.token = ft_lexing(&shell);
	if (!shell.token)
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	result = ft_parse_logical_operators(&shell);
	if (result == -1)
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	ast = shell.ast;
	if (!ast || !ft_validate_logical_ast(ast))
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	ft_cleanup(&shell, 0);
	return (0);
}

/**
 * @brief Tests logical syntax validation
 * 
 * @param test_cmd Command to test (should fail)
 * @param env Environment variables
 * @return int 0 if correctly failed, -1 if incorrectly passed
 */
static int	ft_test_syntax_error(char *test_cmd, char **env)
{
	t_shell	shell;
	bool	valid_syntax;

	if (ft_setup(&shell, env) == -1)
		return (-1);
	shell.current_line = test_cmd;
	shell.token = ft_lexing(&shell);
	if (!shell.token)
	{
		ft_cleanup(&shell, 0);
		return (0);
	}
	valid_syntax = ft_validate_logical_syntax(shell.token);
	ft_cleanup(&shell, 0);
	if (valid_syntax)
		return (-1);
	return (0);
}

/**
 * @brief Main test function
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @param env Environment variables
 * @return int Exit code
 */
int	main(int argc, char **argv, char **env)
{
	int	test_count;
	int	pass_count;

	(void)argc;
	(void)argv;
	test_count = 0;
	pass_count = 0;
	ft_printf("=== Tests Logical Operators Phase 7.1 ===\n\n");
	test_count++;
	if (ft_test_basic_detection() == 0)
	{
		ft_printf("✓ Test 1: Detection basique - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 1: Detection basique - FAIL\n");
	test_count++;
	if (ft_test_precedence_values() == 0)
	{
		ft_printf("✓ Test 2: Valeurs precedence - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 2: Valeurs precedence - FAIL\n");
	test_count++;
	if (ft_test_simple_logical("echo hello && echo world", env) == 0)
	{
		ft_printf("✓ Test 3: Parsing AND simple - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 3: Parsing AND simple - FAIL\n");
	test_count++;
	if (ft_test_simple_logical("true || false", env) == 0)
	{
		ft_printf("✓ Test 4: Parsing OR simple - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 4: Parsing OR simple - FAIL\n");
	test_count++;
	if (ft_test_syntax_error("&& echo test", env) == 0)
	{
		ft_printf("✓ Test 5: Erreur syntaxe debut - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 5: Erreur syntaxe debut - FAIL\n");
	ft_printf("\n=== Résumé ===\n");
	ft_printf("Tests réussis: %d/%d\n", pass_count, test_count);
	if (pass_count == test_count)
	{
		ft_printf("✅ Tous les tests Phase 7.1 sont passés !\n");
		return (0);
	}
	ft_printf("❌ %d test(s) ont échoué.\n", test_count - pass_count);
	return (1);
}
