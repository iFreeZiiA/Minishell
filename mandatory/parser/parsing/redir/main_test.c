/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:20:54 by jjorda           ###   ########.fr       */
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

static int	ft_test_redirection_creation(void)
{
	t_redir	*redir;
	int		result;

	redir = ft_create_redirection(REDIR_OUT, "test.txt");
	if (!redir)
		return (-1);
	result = 0;
	if (redir->type != REDIR_OUT)
		result = -1;
	if (ft_strcmp(redir->file, "test.txt") != 0)
		result = -1;
	free(redir->file);
	free(redir);
	return (result);
}

static int	ft_test_redirection_type_detection(void)
{
	t_token		token;
	redir_type	type;

	token.type = TOKEN_REDIR_OUT;
	type = ft_get_redirection_type(&token);
	if (type != REDIR_OUT)
		return (-1);
	token.type = TOKEN_HEREDOC;
	type = ft_get_redirection_type(&token);
	if (type != REDIR_HEREDOC)
		return (-1);
	token.type = TOKEN_REDIR_IN;
	type = ft_get_redirection_type(&token);
	if (type != REDIR_IN)
		return (-1);
	return (0);
}

static int	ft_test_filename_validation(void)
{
	int	result1;
	int	result2;
	int	result3;

	result1 = ft_validate_filename("valid_file.txt");
	result2 = ft_validate_filename("");
	result3 = ft_validate_filename(NULL);
	if (result1 != 0)
		return (-1);
	if (result2 == 0)
		return (-1);
	if (result3 == 0)
		return (-1);
	return (0);
}

static int	ft_test_redirection_token_detection(void)
{
	t_token	token1;
	t_token	token2;
	t_token	token3;

	token1.type = TOKEN_REDIR_OUT;
	token2.type = TOKEN_WORD;
	token3.type = TOKEN_APPEND;
	if (ft_is_redirection_token(&token1) != 1)
		return (-1);
	if (ft_is_redirection_token(&token2) != 0)
		return (-1);
	if (ft_is_redirection_token(&token3) != 1)
		return (-1);
	return (0);
}

int	main(void)
{
	int	result1;
	int	result2;
	int	result3;
	int	result4;

	ft_printf("=== Tests Parsing Redirections Phase 6.2 ===\n");
	result1 = ft_test_redirection_creation();
	ft_print_test_result("Création redirection", result1);
	result2 = ft_test_redirection_type_detection();
	ft_print_test_result("Détection type redirection", result2);
	result3 = ft_test_filename_validation();
	ft_print_test_result("Validation nom fichier", result3);
	result4 = ft_test_redirection_token_detection();
	ft_print_test_result("Détection token redirection", result4);
	if (result1 == 0 && result2 == 0 && result3 == 0 && result4 == 0)
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
