/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/07/20 18:00:00 by student           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static void	ft_print_token_type(t_token_type type)
{
	if (type == TOKEN_WORD)
		ft_printf("TOKEN_WORD");
	else if (type == TOKEN_PIPE)
		ft_printf("TOKEN_PIPE");
	else if (type == TOKEN_REDIR_OUT)
		ft_printf("TOKEN_REDIR_OUT");
	else if (type == TOKEN_REDIR_IN)
		ft_printf("TOKEN_REDIR_IN");
	else if (type == TOKEN_AND)
		ft_printf("TOKEN_AND");
	else if (type == TOKEN_OR)
		ft_printf("TOKEN_OR");
	else if (type == TOKEN_APPEND)
		ft_printf("TOKEN_APPEND");
	else if (type == TOKEN_DQUOTE)
		ft_printf("TOKEN_DQUOTE");
	else if (type == TOKEN_QUOTE)
		ft_printf("TOKEN_QUOTE");
	else if (type == TOKEN_VAR)
		ft_printf("TOKEN_VAR");
	else if (type == TOKEN_STATUS)
		ft_printf("TOKEN_STATUS");
	else
		ft_printf("TOKEN_OTHER");
}

static void	ft_print_tokens(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		i;

	current = tokens;
	i = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		ft_printf("Token[%d]: ", i);
		ft_print_token_type(token->type);
		ft_printf(" = \"%s\"\n", token->value);
		current = current->next;
		i++;
	}
}

static void	ft_print_ast_info(t_ast_node *ast)
{
	if (!ast)
	{
		ft_printf("AST: NULL\n");
		return ;
	}
	
	if (ast->type == NODE_COMMAND)
	{
		ft_printf("NODE_COMMAND\n");
		// Vérifier si c'est une commande avec pipe intégré
		if (ast->data)
		{
			t_command *cmd = (t_command *)ast->data;
			if (cmd->redirs)
				ft_printf("REDIR_OUT\n");
		}
	}
	else if (ast->type == NODE_PIPE)
		ft_printf("NODE_PIPE\n");
	else if (ast->type == NODE_AND)
		ft_printf("NODE_AND\n");
	else if (ast->type == NODE_OR)
		ft_printf("NODE_OR\n");
	else
		ft_printf("NODE_OTHER\n");
	
	ft_printf("AST_VALID\n");
	
	// Si c'est un nœud composite, vérifier les enfants
	if (ast->left || ast->right)
	{
		ft_printf("COMPOSITE_NODE\n");
		if (ast->left)
			ft_printf("HAS_LEFT_CHILD\n");
		if (ast->right)
			ft_printf("HAS_RIGHT_CHILD\n");
	}
}

static int	ft_process_input(char *input)
{
	t_shell		shell;
	t_list		*tokens;
	int			parse_result;
	char		*test_env[3];

	// Configuration d'un environnement de test sécurisé
	test_env[0] = "TEST_VAR=test_value";
	test_env[1] = "PATH=/usr/bin";
	test_env[2] = NULL;
	
	if (ft_setup(&shell, test_env) < 0)
	{
		ft_printf("Error: Setup failed\n");
		return (1);
	}
	
	if (!input || !input[0])
	{
		ft_printf("Error: Empty input\n");
		return (1);
	}
	
	shell.current_line = input;
	tokens = ft_lexing_simple(&shell);
	if (!tokens)
	{
		ft_printf("Error: Tokenization failed\n");
		return (1);
	}
	
	ft_print_tokens(tokens);
	
	// Utilisation du parser complet avec validation
	shell.token = tokens;
	parse_result = ft_parse_enhanced(&shell);
	
	if (parse_result != 0)
	{
		ft_printf("Error: Parsing failed (validation error)\n");
		if (tokens)
			ft_lstfree_t(tokens);
		return (1);
	}
	
	ft_print_ast_info(shell.ast);
	
	// Nettoyage sécurisé
	if (shell.ast)
		ft_free_ast_node(shell.ast);
	if (tokens)
		ft_lstfree_t(tokens);
	
	return (0);
}

int	main(int argc, char **argv)
{
	char	*input;

	if (argc == 2)
	{
		input = argv[1];
		return (ft_process_input(input));
	}
	else
	{
		ft_printf("Usage: %s \"command line\"\n", argv[0]);
		ft_printf("Example: %s \"echo hello | grep test\"\n", argv[0]);
		return (1);
	}
}
