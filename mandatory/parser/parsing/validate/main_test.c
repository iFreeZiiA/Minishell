/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 16:36:46 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Print test result
 * 
 * @param test_name Name of the test
 * @param result Result (0 = success, -1 = failure)
 */
static void	ft_print_result(char *test_name, int result)
{
	if (result == 0)
		ft_printf("✓ %s - PASS\n", test_name);
	else
		ft_printf("✗ %s - FAIL\n", test_name);
}

/**
 * @brief Test syntax validation
 * 
 * @return int 0 on success, -1 on error
 */
static int	ft_test_syntax_validation(void)
{
	char	*valid_cmd;
	char	*invalid_cmd;
	bool	valid_result;
	bool	invalid_result;

	valid_cmd = "echo hello && echo world";
	valid_result = ft_detect_syntax_error(valid_cmd);
	if (valid_result)
		return (-1);
	invalid_cmd = "echo test &&";
	invalid_result = ft_detect_syntax_error(invalid_cmd);
	if (!invalid_result)
		return (-1);
	invalid_cmd = "| echo test";
	invalid_result = ft_detect_syntax_error(invalid_cmd);
	if (!invalid_result)
		return (-1);
	return (0);
}

/**
 * @brief Test precedence validation
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
 * @brief Test AST validation for complex structures
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
 * @brief Test error detection
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
	invalid_cmd = "echo \"unclosed";
	error_detected = ft_detect_syntax_error(invalid_cmd);
	if (error_detected)
		test_count++;
	invalid_cmd = "(echo test";
	error_detected = ft_detect_syntax_error(invalid_cmd);
	if (error_detected)
		test_count++;
	if (test_count < 4)
		return (-1);
	return (0);
}

/**
 * @brief Test regression on all phases
 * 
 * @param env Environment variables
 * @return int 0 on success, -1 on error
 */
static int	ft_test_regression_phases(char **env)
{
	t_shell	shell;
	char	*test_commands[5];
	int		i;
	int		success_count;

	test_commands[0] = "echo hello world";
	test_commands[1] = "echo \"quoted text\"";
	test_commands[2] = "echo a && echo b";
	test_commands[3] = "echo test | grep test";
	test_commands[4] = "(echo group) && echo after";
	success_count = 0;
	i = 0;
	while (i < 5)
	{
		if (ft_setup(&shell, env) == 0)
		{
			shell.current_line = test_commands[i];
			if (ft_lexing(&shell) && ft_parsing(&shell) == 0)
				success_count++;
			ft_cleanup(&shell, 0);
		}
		i++;
	}
	if (success_count < 4)
		return (-1);
	return (0);
}

/**
 * @brief Test complex command combinations
 * 
 * @param env Environment variables
 * @return int 0 on success, -1 on error
 */
static int	ft_test_complex_combinations(char **env)
{
	t_shell	shell;
	char	*complex_cmds[3];
	int		i;
	int		success_count;

	complex_cmds[0] = "echo a | grep a && echo found || echo not";
	complex_cmds[1] = "(echo test && echo ok) | wc -l";
	complex_cmds[2] = "echo start && (echo mid | cat) && echo end";
	success_count = 0;
	i = 0;
	while (i < 3)
	{
		if (ft_setup(&shell, env) == 0)
		{
			shell.current_line = complex_cmds[i];
			if (ft_lexing(&shell) && ft_parsing(&shell) == 0)
			{
				if (ft_validate_ast_structure(shell.ast))
					success_count++;
			}
			ft_cleanup(&shell, 0);
		}
		i++;
	}
	if (success_count < 2)
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
	ft_printf("=== Tests Validation & Regression Phase 11.6 ===\n\n");
	test_count++;
	if (ft_test_syntax_validation() == 0)
	{
		ft_print_result("Test 1: Validation syntaxique", 0);
		pass_count++;
	}
	else
		ft_print_result("Test 1: Validation syntaxique", -1);
	test_count++;
	if (ft_test_precedence_validation() == 0)
	{
		ft_print_result("Test 2: Validation precedences", 0);
		pass_count++;
	}
	else
		ft_print_result("Test 2: Validation precedences", -1);
	test_count++;
	if (ft_test_ast_validation(env) == 0)
	{
		ft_print_result("Test 3: Validation AST", 0);
		pass_count++;
	}
	else
		ft_print_result("Test 3: Validation AST", -1);
	test_count++;
	if (ft_test_error_detection() == 0)
	{
		ft_print_result("Test 4: Detection erreurs", 0);
		pass_count++;
	}
	else
		ft_print_result("Test 4: Detection erreurs", -1);
	test_count++;
	if (ft_test_regression_phases(env) == 0)
	{
		ft_print_result("Test 5: Regression phases", 0);
		pass_count++;
	}
	else
		ft_print_result("Test 5: Regression phases", -1);
	test_count++;
	if (ft_test_complex_combinations(env) == 0)
	{
		ft_print_result("Test 6: Combinaisons complexes", 0);
		pass_count++;
	}
	else
		ft_print_result("Test 6: Combinaisons complexes", -1);
	ft_printf("\n=== Résumé ===\n");
	ft_printf("Tests réussis: %d/%d\n", pass_count, test_count);
	if (pass_count == test_count)
	{
		ft_printf("✅ Tous les tests Phase 11.6 sont passés !\n");
		return (0);
	}
	ft_printf("❌ %d test(s) ont échoué.\n", test_count - pass_count);
	return (1);
}
