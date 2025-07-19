/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 15:35:47 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static t_token	*ft_create_test_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

static t_list	*ft_create_test_token_list(void)
{
	t_list		*tokens;
	t_token		*token1;
	t_token		*token2;
	t_content	data;

	tokens = NULL;
	token1 = ft_create_test_token(TOKEN_WORD, "echo");
	if (!token1)
		return (NULL);
	token2 = ft_create_test_token(TOKEN_WORD, "hello");
	if (!token2)
	{
		free(token1->value);
		free(token1);
		return (NULL);
	}
	data.token = token1;
	if (!ft_lstadd_back(&tokens, data, TYPE_TOKEN))
	{
		free(token1->value);
		free(token1);
		free(token2->value);
		free(token2);
		return (NULL);
	}
	data.token = token2;
	if (!ft_lstadd_back(&tokens, data, TYPE_TOKEN))
	{
		ft_lstleak(&tokens, ft_clean_node_tok);
		free(token2->value);
		free(token2);
		return (NULL);
	}
	return (tokens);
}

static void	ft_print_test_result(char *test_name, int result)
{
	ft_printf("Test %s: ", test_name);
	if (result == 0)
		ft_printf("✓ PASS\n");
	else
		ft_printf("✗ FAIL\n");
}

static int	ft_test_simple_command_parsing(void)
{
	t_list		*tokens;
	t_shell		shell;
	t_ast_node	*ast;
	t_command	*cmd;

	tokens = ft_create_test_token_list();
	if (!tokens)
		return (-1);
	ast = ft_parser(tokens, &shell);
	if (!ast)
	{
		ft_lstclear(&tokens, free);
		return (-1);
	}
	cmd = (t_command *)ast->data;
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		ft_free_ast_node(ast);
		ft_lstleak(&tokens, ft_clean_node_tok);
		return (-1);
	}
	ft_free_ast_node(ast);
	ft_lstleak(&tokens, ft_clean_node_tok);
	return (0);
}

int	main(void)
{
	int	result;

	ft_printf("=== Tests Parser Phase 5.1 ===\n");
	result = ft_test_simple_command_parsing();
	ft_print_test_result("Simple Command", result);
	if (result == 0)
		ft_printf("Tous les tests sont passés avec succès !\n");
	else
		ft_printf("Certains tests ont échoué.\n");
	return (result);
}
