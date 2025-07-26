/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 09:34:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/* Prototypes locaux pour les tests */
char	*ft_append_line(char *content, char *line);
int		ft_handle_heredoc(t_redir *redir);
int		ft_process_heredocs(t_command *cmd);
char	*ft_read_heredoc_line(char *delimiter);
int		ft_write_to_pipe(char *content, int write_fd);

/**
 * @brief Affiche le résultat d'un test
 * @param test_name Nom du test
 * @param result Résultat (0 = succès)
 */
void	ft_print_test_result(char *test_name, int result)
{
	if (result == 0)
		ft_printf("✅ %s: PASSED\n", test_name);
	else
		ft_printf("❌ %s: FAILED\n", test_name);
}

/**
 * @brief Crée une redirection heredoc de test
 * @param delimiter Délimiteur à utiliser
 * @return t_redir* Redirection créée
 */
t_redir	*ft_create_test_heredoc(char *delimiter)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = REDIR_HEREDOC;
	redir->file = ft_strdup(delimiter);
	redir->fd = -1;
	return (redir);
}

/**
 * @brief Test basique d'append de ligne
 * @return int 0 succès, -1 échec
 */
int	ft_test_append_line(void)
{
	char	*content;
	char	*result;

	content = NULL;
	result = ft_append_line(content, "Hello\n");
	if (!result || ft_strcmp(result, "Hello\n") != 0)
	{
		free(result);
		return (-1);
	}
	content = result;
	result = ft_append_line(content, "World\n");
	if (!result || ft_strcmp(result, "Hello\nWorld\n") != 0)
	{
		free(result);
		return (-1);
	}
	free(result);
	return (0);
}

/**
 * @brief Test de création de structure redirection
 * @return int 0 succès, -1 échec
 */
int	ft_test_create_redir(void)
{
	t_redir	*redir;

	redir = ft_create_test_heredoc("EOF");
	if (!redir)
		return (-1);
	if (redir->type != REDIR_HEREDOC)
	{
		free(redir->file);
		free(redir);
		return (-1);
	}
	if (ft_strcmp(redir->file, "EOF") != 0)
	{
		free(redir->file);
		free(redir);
		return (-1);
	}
	if (redir->fd != -1)
	{
		free(redir->file);
		free(redir);
		return (-1);
	}
	free(redir->file);
	free(redir);
	return (0);
}

/**
 * @brief Point d'entrée des tests
 * @return int 0 si tous réussis, 1 sinon
 */
int	main(void)
{
	int	test1;
	int	test2;
	int	total_failed;

	ft_printf("=== Tests Module Heredoc - Phase 10 ===\n\n");
	test1 = ft_test_append_line();
	ft_print_test_result("Test append line", test1);
	test2 = ft_test_create_redir();
	ft_print_test_result("Test create redirection", test2);
	total_failed = 0;
	if (test1 != 0)
		total_failed++;
	if (test2 != 0)
		total_failed++;
	ft_printf("\n=== Résumé ===\n");
	if (total_failed == 0)
	{
		ft_printf("✅ Tous les tests unitaires sont passés !\n");
		ft_printf("ℹ️  Lancez ./test_heredoc.sh pour les tests d'intégration\n");
		return (0);
	}
	ft_printf("❌ %d test(s) ont échoué.\n", total_failed);
	return (1);
}
