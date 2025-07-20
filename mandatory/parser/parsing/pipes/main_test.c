/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:07:21 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	ft_print_test_result(char *test_name, int result)
{
	ft_printf("Test %s: ", test_name);
	if (result == 0)
		ft_printf("✓ PASS\n");
	else
		ft_printf("✗ FAIL\n");
}

static int	ft_test_basic_functions(void)
{
	t_token	test_token;
	int		pipe_test;

	test_token.type = TOKEN_PIPE;
	test_token.value = ft_strdup("|");
	if (!test_token.value)
		return (-1);
	pipe_test = ft_is_operator_token(&test_token);
	free(test_token.value);
	if (pipe_test == 1)
		return (0);
	return (-1);
}

static int	ft_test_token_validation(void)
{
	t_list	*mock_tokens;
	int		result;

	mock_tokens = NULL;
	result = ft_validate_pipe_syntax(mock_tokens);
	if (result == -1)
		return (0);
	return (-1);
}

static int	ft_test_pipe_counting(void)
{
	if (ft_count_pipes(NULL) == 0)
		return (0);
	return (-1);
}

static int	ft_test_memory_safety(void)
{
	t_ast_node	*null_pipe;

	null_pipe = ft_create_pipe_node(NULL, NULL);
	if (null_pipe == NULL)
		return (0);
	ft_free_ast_node(null_pipe);
	return (-1);
}

int	main(void)
{
	int	result1;
	int	result2;
	int	result3;
	int	result4;

	ft_printf("=== Tests Parser Phase 6.1 - Pipes (Version Sécurisée) ===\n");
	result1 = ft_test_basic_functions();
	ft_print_test_result("Fonctions de base", result1);
	result2 = ft_test_token_validation();
	ft_print_test_result("Validation tokens", result2);
	result3 = ft_test_pipe_counting();
	ft_print_test_result("Comptage pipes", result3);
	result4 = ft_test_memory_safety();
	ft_print_test_result("Sécurité mémoire", result4);
	if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0)
	{
		ft_printf("✅ Tous les tests sécurisés sont passés !\n");
		return (0);
	}
	else
	{
		ft_printf("❌ Certains tests ont échoué.\n");
		return (1);
	}
}
