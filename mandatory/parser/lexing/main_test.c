/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:00:00 by unit_test         #+#    #+#             */
/*   Updated: 2025/07/20 17:38:14 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Print test result with color formatting
 * 
 * @param test_name Name of the test
 * @param result Test result (0 = success, -1 = failure)
 */
void	ft_print_test_result(char *test_name, int result)
{
	if (result == 0)
		ft_printf("✅ %s: PASS\n", test_name);
	else
		ft_printf("❌ %s: FAIL\n", test_name);
}

/**
 * @brief Count tokens of a specific type in the list
 * 
 * @param tokens Token list
 * @param type Token type to count
 * @return int Number of tokens found
 */
int	ft_count_tokens_of_type(t_list *tokens, t_token_type type)
{
	int		count;
	t_token	*token;

	count = 0;
	while (tokens)
	{
		token = tokens->content.token;
		if (token && token->type == type)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

/**
 * @brief Test basic tokenization of simple words
 * 
 * @return int 0 on success, -1 on failure
 */
int	ft_test_basic_tokenization(void)
{
	t_shell	shell;
	t_list	*tokens;
	int		word_count;

	ft_setup(&shell, NULL);
	shell.current_line = "echo hello";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	word_count = ft_count_tokens_of_type(tokens, TOKEN_WORD);
	ft_lstfree_t(tokens);
	return (word_count == 2 ? 0 : -1);
}

/**
 * @brief Test pipe token recognition
 * 
 * @return int 0 on success, -1 on failure
 */
int	ft_test_pipe_tokenization(void)
{
	t_shell	shell;
	t_list	*tokens;
	int		pipe_count;
	int		word_count;

	ft_setup(&shell, NULL);
	shell.current_line = "ls | grep test";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	pipe_count = ft_count_tokens_of_type(tokens, TOKEN_PIPE);
	word_count = ft_count_tokens_of_type(tokens, TOKEN_WORD);
	ft_lstfree_t(tokens);
	return (pipe_count == 1 && word_count >= 3 ? 0 : -1);
}

/**
 * @brief Test redirection out token recognition
 * 
 * @return int 0 on success, -1 on failure
 */
int	ft_test_redirection_out_tokenization(void)
{
	t_shell	shell;
	t_list	*tokens;
	int		redir_count;

	ft_setup(&shell, NULL);
	shell.current_line = "echo hello > file";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	redir_count = ft_count_tokens_of_type(tokens, TOKEN_REDIR_OUT);
	ft_lstfree_t(tokens);
	return (redir_count == 1 ? 0 : -1);
}

/**
 * @brief Test redirection in token recognition
 * 
 * @return int 0 on success, -1 on failure
 */
int	ft_test_redirection_in_tokenization(void)
{
	t_shell	shell;
	t_list	*tokens;
	int		redir_count;

	ft_setup(&shell, NULL);
	shell.current_line = "cat < input";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	redir_count = ft_count_tokens_of_type(tokens, TOKEN_REDIR_IN);
	ft_lstfree_t(tokens);
	return (redir_count == 1 ? 0 : -1);
}

/**
 * @brief Test space handling between tokens
 * 
 * @return int 0 on success, -1 on failure
 */
int	ft_test_spaces_handling(void)
{
	t_shell	shell;
	t_list	*tokens;
	int		word_count;

	ft_setup(&shell, NULL);
	shell.current_line = "echo    hello    world";
	tokens = ft_lexing(&shell);
	if (!tokens)
		return (-1);
	word_count = ft_count_tokens_of_type(tokens, TOKEN_WORD);
	ft_lstfree_t(tokens);
	return (word_count == 3 ? 0 : -1);
}

int	main(void)
{
	int	result1;
	int	result2;
	int	result3;
	int	result4;
	int	result5;

	ft_printf("=== Tests Phase 2 - Lexer Basique ===\n");
	ft_printf("(Tokens: WORD, PIPE, REDIR_OUT, REDIR_IN)\n\n");
	result1 = ft_test_basic_tokenization();
	ft_print_test_result("Tokenization basique", result1);
	result2 = ft_test_pipe_tokenization();
	ft_print_test_result("Reconnaissance pipes", result2);
	result3 = ft_test_redirection_out_tokenization();
	ft_print_test_result("Reconnaissance redir OUT", result3);
	result4 = ft_test_redirection_in_tokenization();
	ft_print_test_result("Reconnaissance redir IN", result4);
	result5 = ft_test_spaces_handling();
	ft_print_test_result("Gestion espaces multiples", result5);
	if (result1 == 0 && result2 == 0 && result3 == 0 
		&& result4 == 0 && result5 == 0)
	{
		ft_printf("\n✅ Phase 2 - Tous les tests réussis !\n");
		ft_printf("Tokens de base reconnus correctement.\n");
		return (0);
	}
	else
	{
		ft_printf("\n❌ Phase 2 - Certains tests ont échoué.\n");
		ft_printf("Vérifiez la tokenisation de base.\n");
		return (1);
	}
}
