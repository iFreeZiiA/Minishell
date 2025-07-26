/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/26 10:05:45 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Tests complex syntax validation
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_complex_validation(void)
{
	char	*test_cmd;
	bool	valid_result;
	int		error_count;

	error_count = 0;
	test_cmd = "echo hello && echo world";
	valid_result = ft_validate_complex_syntax(test_cmd);
	if (!valid_result)
		error_count++;
	test_cmd = "(echo test | grep test) && echo found";
	valid_result = ft_validate_complex_syntax(test_cmd);
	if (!valid_result)
		error_count++;
	test_cmd = "ls *.c | wc -l > output.txt";
	valid_result = ft_validate_complex_syntax(test_cmd);
	if (!valid_result)
		error_count++;
	if (error_count > 0)
		return (-1);
	return (0);
}

/**
 * @brief Tests AST validation for complex structures
 * 
 * @param env Environment variables
 * @return int 0 on success, -1 on error
 */
static int	ft_test_ast_validation(char **env)
{
	t_shell		shell;
	t_ast_node	*ast;
	char		*test_cmd;
	bool		valid_ast;

	if (ft_setup(&shell, env) == -1)
		return (-1);
	test_cmd = "echo hello | grep hello && echo found";
	shell.current_line = test_cmd;
	shell.token = ft_lexing(&shell);
	if (!shell.token)
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	if (ft_parsing(&shell) == -1)
	{
		ft_cleanup(&shell, 0);
		return (-1);
	}
	ast = shell.ast;
	valid_ast = ft_validate_ast_structure(ast);
	ft_cleanup(&shell, 0);
	if (!valid_ast)
		return (-1);
	return (0);
}

/**
 * @brief Tests precedence validation
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_precedence_validation(void)
{
	char	*test_cmd;
	int		precedence_value;
	int		error_count;

	error_count = 0;
	test_cmd = "cmd1 | cmd2 && cmd3";
	precedence_value = ft_get_command_precedence(test_cmd);
	if (precedence_value < 2)
		error_count++;
	test_cmd = "(cmd1 | cmd2) && cmd3";
	precedence_value = ft_get_command_precedence(test_cmd);
	if (precedence_value < 3)
		error_count++;
	if (error_count > 0)
		return (-1);
	return (0);
}

/**
 * @brief Tests error detection
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_error_detection(void)
{
	char	*invalid_cmd;
	bool	error_detected;
	int		test_count;

	test_count = 0;
	invalid_cmd = "echo test &&";
	error_detected = ft_detect_syntax_error(invalid_cmd);
	if (error_detected)
		test_count++;
	invalid_cmd = "| echo test";
	error_detected = ft_detect_syntax_error(invalid_cmd);
	if (error_detected)
		test_count++;
	invalid_cmd = "echo test ||";
	error_detected = ft_detect_syntax_error(invalid_cmd);
	if (error_detected)
		test_count++;
	if (test_count != 3)
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
	ft_printf("=== Tests Validation Phase 11.4 ===\n\n");
	test_count++;
	if (ft_test_complex_validation() == 0)
	{
		ft_printf("✓ Test 1: Validation complexe - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 1: Validation complexe - FAIL\n");
	test_count++;
	if (ft_test_ast_validation(env) == 0)
	{
		ft_printf("✓ Test 2: Validation AST - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 2: Validation AST - FAIL\n");
	test_count++;
	if (ft_test_precedence_validation() == 0)
	{
		ft_printf("✓ Test 3: Validation precedence - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 3: Validation precedence - FAIL\n");
	test_count++;
	if (ft_test_error_detection() == 0)
	{
		ft_printf("✓ Test 4: Detection erreurs - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 4: Detection erreurs - FAIL\n");
	ft_printf("\n=== Résumé ===\n");
	ft_printf("Tests réussis: %d/%d\n", pass_count, test_count);
	if (pass_count == test_count)
	{
		ft_printf("✅ Tous les tests Phase 11.4 sont passés !\n");
		return (0);
	}
	ft_printf("❌ %d test(s) ont échoué.\n", test_count - pass_count);
	return (1);
}
