/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 09:00:01 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Creates a test token with given type and value
 * 
 * @param type Token type
 * @param value Token value
 * @return t_list* New token node
 */
static t_list	*ft_create_test_token(t_token_type type, const char *value)
{
	t_token	*token;
	t_list	*node;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup((char *)value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	node = ft_lstnew_tok(token);
	return (node);
}

/**
 * @brief Tests basic parentheses detection
 * 
 * @return int 0 on success, -1 on failure
 */
static int	ft_test_basic_detection(void)
{
	t_list	*tokens;
	t_list	*open_paren;
	t_list	*close_paren;

	tokens = ft_create_test_token(TOKEN_PAREN_OPEN, "(");
	tokens->next = ft_create_test_token(TOKEN_WORD, "echo");
	tokens->next->next = ft_create_test_token(TOKEN_WORD, "test");
	tokens->next->next->next = ft_create_test_token(TOKEN_PAREN_CLOSE, ")");
	open_paren = tokens;
	close_paren = ft_find_matching_paren(open_paren);
	if (!close_paren)
		return (-1);
	if (close_paren->content.token->type != TOKEN_PAREN_CLOSE)
		return (-1);
	return (0);
}

/**
 * @brief Tests parentheses balance validation
 * 
 * @return int 0 on success, -1 on failure
 */
static int	ft_test_balance_validation(void)
{
	t_list	*tokens;
	int		balance;

	tokens = ft_create_test_token(TOKEN_PAREN_OPEN, "(");
	tokens->next = ft_create_test_token(TOKEN_WORD, "echo");
	tokens->next->next = ft_create_test_token(TOKEN_PAREN_CLOSE, ")");
	balance = ft_validate_parentheses_balance(tokens);
	if (balance != 0)
		return (-1);
	tokens->next->next->next = ft_create_test_token(TOKEN_PAREN_OPEN, "(");
	balance = ft_validate_parentheses_balance(tokens);
	if (balance != 1)
		return (-1);
	return (0);
}

/**
 * @brief Tests content validation between parentheses
 * 
 * @return int 0 on success, -1 on failure
 */
static int	ft_test_content_validation(void)
{
	t_list	*open_paren;
	t_list	*close_paren;
	bool	is_valid;

	open_paren = ft_create_test_token(TOKEN_PAREN_OPEN, "(");
	open_paren->next = ft_create_test_token(TOKEN_WORD, "echo");
	open_paren->next->next = ft_create_test_token(TOKEN_PAREN_CLOSE, ")");
	close_paren = open_paren->next->next;
	is_valid = ft_validate_parentheses_content(open_paren, close_paren);
	if (!is_valid)
		return (-1);
	open_paren->next = close_paren;
	is_valid = ft_validate_parentheses_content(open_paren, close_paren);
	if (is_valid)
		return (-1);
	return (0);
}

/**
 * @brief Tests group parsing functionality
 * 
 * @param env Environment variables
 * @return int 0 on success, -1 on failure
 */
static int	ft_test_group_parsing(char **env)
{
	t_list		*open_paren;
	t_list		*close_paren;
	bool		has_content;

	(void)env;
	open_paren = ft_create_test_token(TOKEN_PAREN_OPEN, "(");
	if (!open_paren)
		return (-1);
	open_paren->next = ft_create_test_token(TOKEN_WORD, "echo");
	if (!open_paren->next)
		return (-1);
	open_paren->next->next = ft_create_test_token(TOKEN_WORD, "hello");
	if (!open_paren->next->next)
		return (-1);
	close_paren = ft_create_test_token(TOKEN_PAREN_CLOSE, ")");
	if (!close_paren)
		return (-1);
	open_paren->next->next->next = close_paren;
	has_content = ft_validate_parentheses_content(open_paren, close_paren);
	if (!has_content)
		return (-1);
	return (0);
}

/**
 * @brief Tests nested parentheses handling
 * 
 * @return int 0 on success, -1 on failure
 */
static int	ft_test_nested_parentheses(void)
{
	t_list	*tokens;
	t_list	*first_open;
	t_list	*matching_close;

	tokens = ft_create_test_token(TOKEN_PAREN_OPEN, "(");
	tokens->next = ft_create_test_token(TOKEN_PAREN_OPEN, "(");
	tokens->next->next = ft_create_test_token(TOKEN_WORD, "echo");
	tokens->next->next->next = ft_create_test_token(TOKEN_PAREN_CLOSE, ")");
	tokens->next->next->next->next = ft_create_test_token(TOKEN_PAREN_CLOSE, ")");
	first_open = tokens;
	matching_close = ft_find_matching_paren(first_open);
	if (!matching_close)
		return (-1);
	if (matching_close != tokens->next->next->next->next)
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
	ft_printf("=== Tests Parentheses Phase 8.1 ===\n\n");
	test_count++;
	if (ft_test_basic_detection() == 0)
	{
		ft_printf("✓ Test 1: Detection basique - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 1: Detection basique - FAIL\n");
	test_count++;
	if (ft_test_balance_validation() == 0)
	{
		ft_printf("✓ Test 2: Validation equilibre - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 2: Validation equilibre - FAIL\n");
	test_count++;
	if (ft_test_content_validation() == 0)
	{
		ft_printf("✓ Test 3: Validation contenu - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 3: Validation contenu - FAIL\n");
	test_count++;
	if (ft_test_group_parsing(env) == 0)
	{
		ft_printf("✓ Test 4: Parsing groupes - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 4: Parsing groupes - FAIL\n");
	test_count++;
	if (ft_test_nested_parentheses() == 0)
	{
		ft_printf("✓ Test 5: Parentheses imbriquees - PASS\n");
		pass_count++;
	}
	else
		ft_printf("✗ Test 5: Parentheses imbriquees - FAIL\n");
	ft_printf("\n=== Résumé ===\n");
	ft_printf("Tests réussis: %d/%d\n", pass_count, test_count);
	if (pass_count == test_count)
	{
		ft_printf("✅ Tous les tests Phase 8.1 sont passés !\n");
		return (0);
	}
	ft_printf("❌ %d test(s) ont échoué.\n", test_count - pass_count);
	return (1);
}
