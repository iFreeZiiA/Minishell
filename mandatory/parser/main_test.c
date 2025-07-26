/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 18:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 22:42:26 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"
// #include "../../../header/parser/integration.h"

/**
 * @brief Test l'intégration complète lexer + parser
 * 
 * @param input Commande à tester
 * @param expected_success Si le test doit réussir ou échouer
 * @return int 0 si test réussi, -1 sinon
 */
static int	ft_test_integration(char *input, bool expected_success)
{
	t_shell	shell;
	int		result;
	char	*test_env[3] = {"TEST_VAR=integration_value", "PATH=/usr/bin", NULL};

	if (ft_setup(&shell, test_env) < 0)
		return (-1);
	
	shell.current_line = input;
	shell.token = ft_lexing(&shell);
	if (!shell.token && expected_success)
		return (-1);
	
	if (shell.token)
		result = ft_parse_enhanced(&shell);
	else
		result = -1;
	
	if (expected_success && result == 0)
	{
		ft_printf("✓ PASS: '%s' - Parsing réussi comme attendu\n", input);
		if (shell.ast)
			ft_free_ast_node(shell.ast);
		if (shell.token)
			ft_lstfree_t(shell.token);
		return (0);
	}
	else if (!expected_success && result != 0)
	{
		ft_printf("✓ PASS: '%s' - Échec attendu\n", input);
		return (0);
	}
	else
	{
		ft_printf("✗ FAIL: '%s' - Résultat inattendu\n", input);
		if (shell.ast)
			ft_free_ast_node(shell.ast);
		if (shell.token)
			ft_lstfree_t(shell.token);
		return (-1);
	}
}

/**
 * @brief Tests de commandes simples
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_simple_commands(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS COMMANDES SIMPLES ===\n");
	
	if (ft_test_integration("echo hello", true) != 0)
		failed++;
	if (ft_test_integration("pwd", true) != 0)
		failed++;
	if (ft_test_integration("echo hello world", true) != 0)
		failed++;
	if (ft_test_integration("echo \"hello world\"", true) != 0)
		failed++;
	if (ft_test_integration("echo 'hello world'", true) != 0)
		failed++;
	
	return (failed);
}

/**
 * @brief Tests de pipes
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_pipes(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS PIPES ===\n");
	
	if (ft_test_integration("echo hello | cat", true) != 0)
		failed++;
	if (ft_test_integration("ls | grep test", true) != 0)
		failed++;
	if (ft_test_integration("echo hello | cat | wc -l", true) != 0)
		failed++;
	if (ft_test_integration("echo hello |", false) != 0)  // Doit échouer
		failed++;
	if (ft_test_integration("| echo hello", false) != 0) // Doit échouer
		failed++;
	
	return (failed);
}

/**
 * @brief Tests d'opérateurs logiques
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_logical_operators(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS OPÉRATEURS LOGIQUES ===\n");
	
	if (ft_test_integration("echo a && echo b", true) != 0)
		failed++;
	if (ft_test_integration("echo a || echo b", true) != 0)
		failed++;
	if (ft_test_integration("true && echo success", true) != 0)
		failed++;
	if (ft_test_integration("false || echo backup", true) != 0)
		failed++;
	if (ft_test_integration("echo hello && echo world || echo failed", true) != 0)
		failed++;
	if (ft_test_integration("echo hello &&", false) != 0) // Doit échouer
		failed++;
	if (ft_test_integration("|| echo hello", false) != 0) // Doit échouer
		failed++;
	
	return (failed);
}

/**
 * @brief Tests de redirections
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_redirections(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS REDIRECTIONS ===\n");
	
	if (ft_test_integration("echo hello > output.txt", true) != 0)
		failed++;
	if (ft_test_integration("cat < input.txt", true) != 0)
		failed++;
	if (ft_test_integration("echo hello >> append.txt", true) != 0)
		failed++;
	if (ft_test_integration("cat << EOF", true) != 0)
		failed++;
	if (ft_test_integration("echo hello >", false) != 0) // Doit échouer
		failed++;
	if (ft_test_integration("> echo hello", false) != 0) // Doit échouer
		failed++;
	
	return (failed);
}

/**
 * @brief Tests de parenthèses
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_parentheses(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS PARENTHÈSES ===\n");
	
	if (ft_test_integration("(echo hello)", true) != 0)
		failed++;
	if (ft_test_integration("(echo a && echo b)", true) != 0)
		failed++;
	if (ft_test_integration("(echo hello) | cat", true) != 0)
		failed++;
	if (ft_test_integration("(echo a && echo b) || echo c", true) != 0)
		failed++;
	if (ft_test_integration("(echo hello", false) != 0) // Doit échouer
		failed++;
	if (ft_test_integration("echo hello)", false) != 0) // Doit échouer
		failed++;
	
	return (failed);
}

/**
 * @brief Tests d'expansion
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_expansion(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS EXPANSION ===\n");
	
	if (ft_test_integration("echo $TEST_VAR", true) != 0)
		failed++;
	if (ft_test_integration("echo \"$TEST_VAR\"", true) != 0)
		failed++;
	if (ft_test_integration("echo '$TEST_VAR'", true) != 0)
		failed++;
	if (ft_test_integration("echo $?", true) != 0)
		failed++;
	if (ft_test_integration("echo ${TEST_VAR}", true) != 0)
		failed++;
	
	return (failed);
}

/**
 * @brief Tests complexes combinant plusieurs fonctionnalités
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_complex_commands(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS COMMANDES COMPLEXES ===\n");
	
	// Combinaison pipe + logique
	if (ft_test_integration("echo hello | cat && echo success", true) != 0)
		failed++;
	
	// Combinaison parenthèses + logique
	if (ft_test_integration("(echo a && echo b) || echo c", true) != 0)
		failed++;
	
	// Combinaison expansion + pipe
	if (ft_test_integration("echo $TEST_VAR | cat", true) != 0)
		failed++;
	
	// Combinaison quotes + expansion + logique
	if (ft_test_integration("echo \"$TEST_VAR\" && echo success", true) != 0)
		failed++;
	
	// Commande ultra-complexe
	if (ft_test_integration("(echo $TEST_VAR && echo world) | cat || echo failed", true) != 0)
		failed++;
	
	// Test avec wildcards (si supporté)
	if (ft_test_integration("echo *.c", true) != 0)
		failed++;
	
	return (failed);
}

/**
 * @brief Tests de gestion d'erreurs
 * 
 * @return int Nombre de tests échoués
 */
static int	ft_test_error_handling(void)
{
	int	failed = 0;

	ft_printf("\n=== TESTS GESTION D'ERREURS ===\n");
	
	// Input vide
	if (ft_test_integration("", false) != 0)
		failed++;
	
	// Quotes non fermées
	if (ft_test_integration("echo \"hello", false) != 0)
		failed++;
	if (ft_test_integration("echo 'hello", false) != 0)
		failed++;
	
	// Opérateurs mal placés
	if (ft_test_integration("echo hello &&", false) != 0)
		failed++;
	if (ft_test_integration("&& echo hello", false) != 0)
		failed++;
	if (ft_test_integration("echo hello ||", false) != 0)
		failed++;
	if (ft_test_integration("|| echo hello", false) != 0)
		failed++;
	
	// Pipes mal placés
	if (ft_test_integration("echo hello |", false) != 0)
		failed++;
	if (ft_test_integration("| echo hello", false) != 0)
		failed++;
	
	// Parenthèses déséquilibrées
	if (ft_test_integration("(echo hello", false) != 0)
		failed++;
	if (ft_test_integration("echo hello)", false) != 0)
		failed++;
	if (ft_test_integration("((echo hello)", false) != 0)
		failed++;
	
	return (failed);
}

/**
 * @brief Fonction principale de test
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
	
	ft_printf("==========================================================\n");
	ft_printf("            TESTS D'INTÉGRATION PARSING\n");
	ft_printf("              Phases 2-11.4 Complètes\n");
	ft_printf("==========================================================\n");
	
	// Lancement de tous les tests
	total_failed += ft_test_simple_commands();
	total_failed += ft_test_pipes();
	total_failed += ft_test_logical_operators();
	total_failed += ft_test_redirections();
	total_failed += ft_test_parentheses();
	total_failed += ft_test_expansion();
	total_failed += ft_test_complex_commands();
	total_failed += ft_test_error_handling();
	
	// Résumé final
	ft_printf("\n==========================================================\n");
	ft_printf("                      RÉSUMÉ\n");
	ft_printf("==========================================================\n");
	
	if (total_failed == 0)
	{
		ft_printf("🎉 ✅ TOUS LES TESTS D'INTÉGRATION SONT PASSÉS !\n");
		ft_printf("✅ Le parsing intégré est fonctionnel\n");
		ft_printf("✅ Prêt pour l'intégration dans main.c\n");
		ft_printf("==========================================================\n");
		return (0);
	}
	else
	{
		ft_printf("❌ %d test(s) d'intégration ont échoué\n", total_failed);
		ft_printf("⚠️  Vérifiez les fonctions défaillantes avant intégration\n");
		ft_printf("==========================================================\n");
		return (1);
	}
}

