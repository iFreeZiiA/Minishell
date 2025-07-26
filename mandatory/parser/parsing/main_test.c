/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/20 17:30:00 by student           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static void	ft_print_result(char *test, int result)
{
	ft_printf("Test %s: ", test);
	if (result == 0)
		ft_printf("✓ PASS\n");
	else
		ft_printf("✗ FAIL\n");
}

static int	ft_test_phase2_tokens(void)
{
	t_shell		shell;
	t_list		*tokens;
	t_token		*token;

	ft_setup(&shell, NULL);
	shell.current_line = "echo hello world";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	token = (t_token *)tokens->content.token;
	if (token->type != TOKEN_WORD)
	{
		ft_lstfree_t(tokens);
		return (-1);
	}
	if (ft_strcmp(token->value, "echo") != 0)
	{
		ft_lstfree_t(tokens);
		return (-1);
	}
	ft_lstfree_t(tokens);
	return (0);
}

static int	ft_test_phase3_quotes(void)
{
	t_shell		shell;
	t_list		*tokens;
	t_token		*token;
	char		*found;

	ft_setup(&shell, NULL);
	shell.current_line = "echo \"hello world\"";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	
	// Recherche du token contenant "hello world"
	// Il peut être TOKEN_DQUOTE ou TOKEN_WORD selon votre implémentation
	token = (t_token *)tokens->next->content.token;
	
	// Vérification que le contenu est préservé (avec ou sans quotes)
	found = ft_strnstr(token->value, "hello world", ft_strlen(token->value));
	if (found == NULL)
	{
		ft_lstfree_t(tokens);
		return (-1);
	}
	
	ft_lstfree_t(tokens);
	return (0);
}

static int	ft_test_phase3_expansion(void)
{
	t_shell		shell;
	t_list		*tokens;
	t_token		*token;
	char		*test_env[3];
	int			result;

	test_env[0] = "TEST=value";
	test_env[1] = "PATH=/usr/bin";
	test_env[2] = NULL;
	ft_setup(&shell, test_env);
	shell.current_line = "echo $TEST";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	token = (t_token *)tokens->next->content.token;
	result = 0;
	if (ft_strncmp(token->value, "value", 5) != 0)
		result = -1;
	ft_lstfree_t(tokens);
	return (result);
}

static int	ft_test_phase4_operators(void)
{
	t_shell		shell;
	t_list		*tokens;
	t_token		*token;
	t_list		*current;

	ft_setup(&shell, NULL);
	shell.current_line = "echo hello && echo world";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	current = tokens->next->next;
	if (!current)
	{
		ft_lstfree_t(tokens);
		return (-1);
	}
	token = (t_token *)current->content.token;
	if (token->type != TOKEN_AND)
	{
		ft_lstfree_t(tokens);
		return (-1);
	}
	ft_lstfree_t(tokens);
	return (0);
}

static int	ft_test_phase5_parser(void)
{
	t_shell		shell;
	t_list		*tokens;
	t_ast_node	*ast;

	ft_setup(&shell, NULL);
	shell.current_line = "echo hello";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	ast = ft_parser(tokens, &shell);
	if (!ast)
	{
		ft_lstfree_t(tokens);
		return (-1);
	}
	if (ast->type != NODE_COMMAND)
	{
		ft_free_ast_node(ast);
		ft_lstfree_t(tokens);
		return (-1);
	}
	ft_free_ast_node(ast);
	ft_lstfree_t(tokens);
	return (0);
}

int	main(void)
{
	int	test1;
	int	test2;
	int	test3;
	int	test4;
	int	test5;

	ft_printf("=== Tests Phases 2-5 Minishell ===\n");
	test1 = ft_test_phase2_tokens();
	ft_print_result("Phase 2 - Tokenisation", test1);
	test2 = ft_test_phase3_quotes();
	ft_print_result("Phase 3 - Quotes", test2);
	test3 = ft_test_phase3_expansion();
	ft_print_result("Phase 3 - Expansion", test3);
	test4 = ft_test_phase4_operators();
	ft_print_result("Phase 4 - Opérateurs", test4);
	test5 = ft_test_phase5_parser();
	ft_print_result("Phase 5 - Parser", test5);
	if (test1 == 0 && test2 == 0 && test3 == 0 && test4 == 0 && test5 == 0)
	{
		ft_printf("✅ Tous les tests sont passés !\n");
		return (0);
	}
	ft_printf("❌ Certains tests ont échoué.\n");
	return (1);
}
