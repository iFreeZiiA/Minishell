/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_integration_test.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 17:21:35 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/**
 * @brief Test intégration lexer + parser simplifié
 * 
 * @param input Commande à tester
 * @return int 0 si succès, -1 si erreur
 */
static int	ft_test_lexer_parser_integration(char *input)
{
	t_shell	shell;
	t_list	*tokens;
	t_ast_node	*ast;
	char	*test_env[3] = {"TEST_VAR=value", "PATH=/usr/bin", NULL};

	ft_printf("Test: '%s'\n", input);
	
	if (ft_setup(&shell, test_env) < 0)
		return (-1);
	
	shell.current_line = input;
	tokens = ft_lexing(&shell);
	if (!tokens)
	{
		ft_printf("  ✗ Lexing failed\n");
		return (-1);
	}
	
	shell.token = tokens;
	if (ft_parse(&shell) != 0)
	{
		ft_printf("  ✗ Parsing failed\n");
		ft_lstfree_t(tokens);
		return (-1);
	}
	
	ast = shell.ast;
	if (!ast)
	{
		ft_printf("  ✗ No AST generated\n");
		ft_lstfree_t(tokens);
		return (-1);
	}
	
	ft_printf("  ✓ Lexing + Parsing successful\n");
	
	// Cleanup
	ft_free_ast_node(ast);
	ft_lstfree_t(tokens);
	
	return (0);
}

/**
 * @brief Lance une série de tests d'intégration
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_run_integration_tests(void)
{
	int	failed = 0;
	char	*test_commands[] = {
		"echo hello",
		"echo hello world",
		"echo \"hello world\"",
		"echo 'hello world'",
		"echo hello | cat",
		"echo hello > file",
		"true && echo success",
		"false || echo backup",
		NULL
	};
	int	i = 0;

	ft_printf("=== TESTS D'INTÉGRATION PARSING ===\n\n");
	
	while (test_commands[i])
	{
		if (ft_test_lexer_parser_integration(test_commands[i]) != 0)
			failed++;
		ft_printf("\n");
		i++;
	}
	
	return (failed);
}

/**
 * @brief Test de gestion d'erreur
 * 
 * @param input Commande qui doit échouer
 * @return int 0 si échec attendu, -1 si succès inattendu
 */
static int	ft_test_error_handling(char *input)
{
	t_shell	shell;
	char	*test_env[3] = {"TEST_VAR=value", "PATH=/usr/bin", NULL};

	ft_printf("Test erreur: '%s'\n", input);
	
	if (ft_setup(&shell, test_env) < 0)
		return (-1);
	
	shell.current_line = input;
	shell.token = ft_lexing(&shell);
	
	if (shell.token && ft_parse(&shell) == 0)
	{
		ft_printf("  ✗ Devrait échouer mais a réussi\n");
		if (shell.ast)
			ft_free_ast_node(shell.ast);
		if (shell.token)
			ft_lstfree_t(shell.token);
		return (-1);
	}
	
	ft_printf("  ✓ Échec attendu\n");
	
	// Cleanup si nécessaire
	if (shell.token)
		ft_lstfree_t(shell.token);
	
	return (0);
}

/**
 * @brief Test des cas d'erreur
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_run_error_tests(void)
{
	int	failed = 0;
	char	*error_commands[] = {
		"echo hello &&",
		"echo hello ||",
		"echo hello |",
		"&& echo hello",
		"|| echo hello",
		"| echo hello",
		NULL
	};
	int	i = 0;

	ft_printf("=== TESTS GESTION D'ERREURS ===\n\n");
	
	while (error_commands[i])
	{
		if (ft_test_error_handling(error_commands[i]) != 0)
			failed++;
		ft_printf("\n");
		i++;
	}
	
	return (failed);
}

/**
 * @brief Fonction principale
 * 
 * @param argc Nombre d'arguments
 * @param argv Arguments
 * @param env Variables d'environnement
 * @return int Code de sortie
 */
int	main(int argc, char **argv, char **env)
{
	int	total_failed = 0;

	(void)argc;
	(void)argv;
	(void)env;
	
	ft_printf("=====================================================\n");
	ft_printf("        TESTS INTÉGRATION PARSING SIMPLE\n");
	ft_printf("=====================================================\n\n");
	
	total_failed += ft_run_integration_tests();
	total_failed += ft_run_error_tests();
	
	ft_printf("=====================================================\n");
	ft_printf("                    RÉSUMÉ\n");
	ft_printf("=====================================================\n");
	
	if (total_failed == 0)
	{
		ft_printf("🎉 ✅ TOUS LES TESTS D'INTÉGRATION SONT PASSÉS !\n");
		ft_printf("✅ Le parsing intégré fonctionne correctement\n");
		return (0);
	}
	else
	{
		ft_printf("❌ %d test(s) d'intégration ont échoué\n", total_failed);
		ft_printf("⚠️  Vérifiez les fonctions défaillantes\n");
		return (1);
	}
}
