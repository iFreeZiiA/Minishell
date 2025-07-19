/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 14:41:32 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Prints token list for debugging
 * 
 * @param tok_h Head of token list
 * @param title Title for the output
 */
static void	ft_print_tokens(t_list *tok_h, const char *title)
{
	t_list	*curr;
	t_token	*token;

	ft_printf("=== %s ===\n", title);
	if (!tok_h)
	{
		ft_printf("(empty list)\n\n");
		return ;
	}
	curr = tok_h;
	while (curr)
	{
		token = curr->content.token;
		if (token)
			ft_printf("'%s' ", token->value);
		curr = curr->next;
	}
	ft_printf("\n\n");
}

/**
 * @brief Tests wildcard detection function
 * 
 * @param test_cases Array of test strings
 * @param count Number of test cases
 */
static void	ft_test_wildcard_detection(char **test_cases, int count)
{
	t_token	token;
	int		i;
	bool	result;

	ft_printf("=== TEST DETECTION WILDCARDS ===\n");
	i = 0;
	while (i < count)
	{
		token.type = TOKEN_WORD;
		token.value = test_cases[i];
		result = ft_needs_wildcard_expansion(&token);
		ft_printf("'%s' -> %s\n", test_cases[i], 
			result ? "WILDCARD" : "NO WILDCARD");
		i++;
	}
	ft_printf("\n");
}

/**
 * @brief Tests pattern matching function
 * 
 * @param patterns Array of patterns
 * @param strings Array of strings to match
 * @param count Number of tests
 */
static void	ft_test_pattern_matching(char **patterns, char **strings, int count)
{
	int		i;
	bool	result;

	ft_printf("=== TEST PATTERN MATCHING ===\n");
	i = 0;
	while (i < count)
	{
		result = ft_match_pattern(patterns[i], strings[i]);
		ft_printf("Pattern: '%s' String: '%s' -> %s\n", 
			patterns[i], strings[i], result ? "MATCH" : "NO MATCH");
		i++;
	}
	ft_printf("\n");
}

/**
 * @brief Tests wildcard expansion on a complete command
 * 
 * @param shell Shell structure
 * @param test_cmd Test command string
 * @return int 0 on success, 1 on error
 */
static int	ft_test_wildcard_expansion(t_shell *shell, const char *test_cmd)
{
	t_list	*tokens_before;
	t_list	*tokens_after;

	ft_printf("=== TEST EXPANSION COMPLETE ===\n");
	ft_printf("Commande: '%s'\n\n", test_cmd);
	shell->current_line = (char *)test_cmd;
	tokens_before = ft_lexing(shell);
	if (!tokens_before)
	{
		ft_printf("✗ Echec du lexing\n");
		return (1);
	}
	ft_print_tokens(tokens_before, "TOKENS AVANT EXPANSION");
	tokens_after = shell->token;
	ft_print_tokens(tokens_after, "TOKENS APRES EXPANSION");
	ft_printf("✓ Test expansion complete\n\n");
	return (0);
}

/**
 * @brief Main function for wildcard testing
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @param env Environment variables
 * @return int Exit code
 */
int	main(int argc, char **argv, char **env)
{
	t_shell		shell;
	char		*detection_tests[5];
	char		*pattern_tests[4];
	char		*string_tests[4];

	if (argc > 2)
	{
		ft_printf("Usage: %s [\"commande avec wildcards\"]\n", argv[0]);
		ft_printf("Exemples:\n");
		ft_printf("  %s \"echo *.c\"\n", argv[0]);
		ft_printf("  %s \"ls *test*\"\n", argv[0]);
		return (1);
	}
	if (ft_setup(&shell, env) == -1)
	{
		ft_printerr("Erreur: Initialisation echouee\n");
		return (1);
	}
	ft_printf("=== TEST MODULE WILDCARD ===\n\n");
	detection_tests[0] = "*.c";
	detection_tests[1] = "test*";
	detection_tests[2] = "*file*";
	detection_tests[3] = "hello.txt";
	detection_tests[4] = "no_wildcard";
	ft_test_wildcard_detection(detection_tests, 5);
	pattern_tests[0] = "*.c";
	pattern_tests[1] = "test*";
	pattern_tests[2] = "*file*";
	pattern_tests[3] = "exact";
	string_tests[0] = "main.c";
	string_tests[1] = "test_wildcard";
	string_tests[2] = "myfilename";
	string_tests[3] = "exact";
	ft_test_pattern_matching(pattern_tests, string_tests, 4);
	if (argc == 2)
	{
		if (ft_test_wildcard_expansion(&shell, argv[1]) != 0)
		{
			ft_cleanup(&shell, 1);
			return (1);
		}
	}
	ft_printf("=== TOUS LES TESTS TERMINES ===\n");
	ft_cleanup(&shell, 0);
	return (0);
}
