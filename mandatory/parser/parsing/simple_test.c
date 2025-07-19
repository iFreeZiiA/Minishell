/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 15:37:28 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static void	ft_print_test_result(char *test_name, int result)
{
	ft_printf("Test %s: ", test_name);
	if (result == 0)
		ft_printf("✓ PASS\n");
	else
		ft_printf("✗ FAIL\n");
}

static int	ft_test_parser_with_null(void)
{
	t_shell		shell;
	t_ast_node	*result;

	result = ft_parser(NULL, &shell);
	if (result == NULL)
		return (0);
	ft_free_ast_node(result);
	return (-1);
}

static int	ft_test_validation_functions(void)
{
	t_token	test_token;
	int		word_test;
	int		operator_test;

	test_token.type = TOKEN_WORD;
	word_test = ft_is_word_token(&test_token);
	test_token.type = TOKEN_PIPE;
	operator_test = ft_is_operator_token(&test_token);
	if (word_test == 1 && operator_test == 1)
		return (0);
	return (-1);
}

static int	ft_test_ast_validation(void)
{
	t_ast_node	*ast;
	t_command	*cmd;
	char		**args;
	int			result;

	ast = malloc(sizeof(t_ast_node));
	if (!ast)
		return (-1);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		free(ast);
		return (-1);
	}
	args = malloc(sizeof(char *) * 2);
	if (!args)
	{
		free(cmd);
		free(ast);
		return (-1);
	}
	args[0] = ft_strdup("echo");
	args[1] = NULL;
	cmd->args = args;
	cmd->redirs = NULL;
	ast->type = NODE_COMMAND;
	ast->data = cmd;
	ast->left = NULL;
	ast->right = NULL;
	result = ft_validate_ast_structure(ast);
	ft_free_ast_node(ast);
	return (result);
}

int	main(void)
{
	int	result1;
	int	result2;
	int	result3;

	ft_printf("=== Tests Parser Phase 5.1 (Version Simplifiée) ===\n");
	result1 = ft_test_parser_with_null();
	ft_print_test_result("Parser avec NULL", result1);
	result2 = ft_test_validation_functions();
	ft_print_test_result("Fonctions de validation", result2);
	result3 = ft_test_ast_validation();
	ft_print_test_result("Validation AST", result3);
	if (result1 == 0 && result2 == 0 && result3 == 0)
	{
		ft_printf("✅ Tous les tests sont passés avec succès !\n");
		return (0);
	}
	else
	{
		ft_printf("❌ Certains tests ont échoué.\n");
		return (1);
	}
}
