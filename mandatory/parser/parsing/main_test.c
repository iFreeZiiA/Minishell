/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:49:47 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Initializes shell structure for testing
 * 
 * @param shell Shell structure to initialize
 * @param env Environment variables
 * @return int 0 on success, -1 on error
 */
static int	ft_init_test_shell(t_shell *shell, char **env)
{
	if (!shell)
		return (-1);
	shell->env->env_vars = env;
	shell->token = NULL;
	shell->ast = NULL;
	shell->env->last_exit_code = 0;
	shell->current_line = NULL;
	return (0);
}

/**
 * @brief Runs lexical analysis test
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
static int	ft_test_lexing(t_shell *shell)
{
	ft_printf("=== ÉTAPE 1: ANALYSE LEXICALE ===\n");
	shell->token = ft_lexing(shell);
	if (!shell->token)
	{
		ft_printerr("Erreur: Analyse lexicale échouée\n");
		return (-1);
	}
	ft_print_token_list(shell->token);
	return (0);
}

/**
 * @brief Runs parsing test
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
static int	ft_test_parsing(t_shell *shell)
{
	int	result;

	ft_printf("=== ÉTAPE 2: ANALYSE SYNTAXIQUE ===\n");
	result = ft_parsing(shell);
	if (result < 0 || !shell->ast)
	{
		ft_printerr("Erreur: Analyse syntaxique échouée (code %d)\n", result);
		return (-1);
	}
	ft_printf("✓ Parsing réussi\n");
	return (0);
}

/**
 * @brief Displays parsing results
 * 
 * @param shell Shell structure
 */
static void	ft_display_results(t_shell *shell)
{
	ft_printf("\n=== ARBRE SYNTAXIQUE ABSTRAIT ===\n");
	if (shell->ast)
		ft_print_ast(shell->ast, 0);
	else
		ft_printf("Aucun AST généré\n");
	ft_printf("\nAnalyse terminée avec succès.\n");
}

/**
 * @brief Main test function
 * 
 * @param argc Argument count
 * @param argv Arguments array
 * @param env Environment variables
 * @return int Exit code
 */
int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	if (argc != 2)
	{
		ft_printf("Usage: %s \"commande à analyser\"\n", argv[0]);
		ft_printf("Exemple: %s \"echo hello && ls | grep test\"\n", argv[0]);
		return (1);
	}
	if (ft_init_test_shell(&shell, env) < 0)
	{
		ft_printerr("Erreur: Initialisation échouée\n");
		return (1);
	}
	shell.current_line = argv[1];
	ft_printf("=== ANALYSE DE LA COMMANDE ===\n");
	ft_printf("\"%s\"\n\n", shell.current_line);
	if (ft_test_lexing(&shell) < 0)
		return (1);
	if (ft_test_parsing(&shell) < 0)
	{
		ft_lstfree_t(shell.token);
		return (1);
	}
	ft_display_results(&shell);
	// ft_free_ast(shell.ast);
	ft_lstfree_t(shell.token);
	return (0);
}
