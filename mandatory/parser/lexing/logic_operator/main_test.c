/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:20:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/29 18:37:53 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Counts logical operators in token list
 * 
 * @param shell Shell structure
 */
static void	ft_count_logical_operators(t_shell *shell)
{
	t_list	*curr;
	int		and_count;
	int		or_count;
	int		pipe_count;

	ft_printf("=== ANALYSE DES OPERATEURS ===\n");
	curr = shell->token;
	and_count = 0;
	or_count = 0;
	pipe_count = 0;
	while (curr)
	{
		if (curr->content.token->type == TOKEN_AND)
			and_count++;
		else if (curr->content.token->type == TOKEN_OR)
			or_count++;
		else if (curr->content.token->type == TOKEN_PIPE)
			pipe_count++;
		curr = curr->next;
	}
	ft_printf("Operateurs && (AND): %d\n", and_count);
	ft_printf("Operateurs || (OR): %d\n", or_count);
	ft_printf("Operateurs | (PIPE): %d\n", pipe_count);
	ft_printf("Total operateurs: %d\n\n", and_count + or_count + pipe_count);
}

/**
 * @brief Validates logical operator syntax
 * 
 * @param shell Shell structure
 * @return bool true if valid, false otherwise
 */
static bool	ft_validate_logical_syntax(t_shell *shell)
{
	t_list	*curr;
	bool	valid;
	bool	prev_was_operator;

	ft_printf("=== VALIDATION DE LA SYNTAXE ===\n");
	curr = shell->token;
	valid = true;
	prev_was_operator = false;
	while (curr && valid)
	{
		if (ft_is_logical_operator(curr->content.token))
		{
			if (prev_was_operator || !curr->next)
			{
				ft_printf("✗ Erreur: Operateur mal place\n");
				valid = false;
			}
			prev_was_operator = true;
		}
		else if (curr->content.token->type == TOKEN_WORD)
			prev_was_operator = false;
		curr = curr->next;
	}
	if (valid)
		ft_printf("✓ Syntaxe des operateurs valide\n");
	ft_printf("\n");
	return (valid);
}

/**
 * @brief Checks if token is a logical operator
 * 
 * @param token Token to check
 * @return bool true if logical operator, false otherwise
 */
static bool	ft_is_logical_operator(t_token *token)
{
	if (!token)
		return (false);
	return (token->type == TOKEN_AND || token->type == TOKEN_OR 
		|| token->type == TOKEN_PIPE);
}

/**
 * @brief Prints detailed AST structure for logical operators
 * 
 * @param ast AST node to print
 * @param level Indentation level
 */
static void	ft_print_logical_ast_detailed(t_ast_node *ast, int level)
{
	int	i;

	if (!ast)
		return ;
	i = 0;
	while (i < level)
	{
		ft_printf("  ");
		i++;
	}
	if (ast->type == NODE_AND)
		ft_print_and_node(ast, level);
	else if (ast->type == NODE_OR)
		ft_print_or_node(ast, level);
	else if (ast->type == NODE_PIPE)
		ft_print_pipe_node(ast, level);
	else if (ast->type == NODE_COMMAND)
		ft_print_command_node_detailed(ast);
	else
		ft_printf("NODE_TYPE: %d\n", ast->type);
}

/**
 * @brief Prints AND node details
 * 
 * @param ast AST node
 * @param level Indentation level
 */
static void	ft_print_and_node(t_ast_node *ast, int level)
{
	ft_printf("AND (&&) - Court-circuit si echec\n");
	if (ast->left)
	{
		ft_print_indent(level + 1);
		ft_printf("GAUCHE:\n");
		ft_print_logical_ast_detailed(ast->left, level + 2);
	}
	if (ast->right)
	{
		ft_print_indent(level + 1);
		ft_printf("DROITE:\n");
		ft_print_logical_ast_detailed(ast->right, level + 2);
	}
}

/**
 * @brief Prints OR node details
 * 
 * @param ast AST node
 * @param level Indentation level
 */
static void	ft_print_or_node(t_ast_node *ast, int level)
{
	ft_printf("OR (||) - Court-circuit si succes\n");
	if (ast->left)
	{
		ft_print_indent(level + 1);
		ft_printf("GAUCHE:\n");
		ft_print_logical_ast_detailed(ast->left, level + 2);
	}
	if (ast->right)
	{
		ft_print_indent(level + 1);
		ft_printf("DROITE:\n");
		ft_print_logical_ast_detailed(ast->right, level + 2);
	}
}

/**
 * @brief Prints PIPE node details
 * 
 * @param ast AST node
 * @param level Indentation level
 */
static void	ft_print_pipe_node(t_ast_node *ast, int level)
{
	ft_printf("PIPE (|) - Redirection stdout->stdin\n");
	if (ast->left)
	{
		ft_print_indent(level + 1);
		ft_printf("PRODUCTEUR:\n");
		ft_print_logical_ast_detailed(ast->left, level + 2);
	}
	if (ast->right)
	{
		ft_print_indent(level + 1);
		ft_printf("CONSOMMATEUR:\n");
		ft_print_logical_ast_detailed(ast->right, level + 2);
	}
}

/**
 * @brief Prints command node with details
 * 
 * @param ast AST node
 */
static void	ft_print_command_node_detailed(t_ast_node *ast)
{
	t_command	*cmd;
	int			i;

	cmd = (t_command *)ast->data;
	ft_printf("COMMANDE: ");
	if (cmd && cmd->args && cmd->args[0])
	{
		i = 0;
		while (cmd->args[i])
		{
			ft_printf("'%s'", cmd->args[i]);
			if (cmd->args[i + 1])
				ft_printf(" ");
			i++;
		}
	}
	else
		ft_printf("(vide)");
	ft_printf("\n");
}

/**
 * @brief Prints indentation
 * 
 * @param level Indentation level
 */
static void	ft_print_indent(int level)
{
	int	i;

	i = 0;
	while (i < level)
	{
		ft_printf("  ");
		i++;
	}
}

/**
 * @brief Tests logical operator parsing
 * 
 * @param shell Shell structure
 * @return int 0 on success, 1 on error
 */
static int	ft_test_logical_parsing(t_shell *shell)
{
	int	parse_result;

	ft_printf("=== TEST DU PARSING LOGIQUE ===\n");
	parse_result = ft_parsing_with_logical(shell);
	if (parse_result < 0)
	{
		ft_printf("✗ Echec du parsing (code: %d)\n", parse_result);
		return (1);
	}
	ft_printf("✓ Parsing des operateurs logiques reussi\n\n");
	if (shell->ast)
	{
		ft_printf("=== ARBRE SYNTAXIQUE LOGIQUE ===\n");
		ft_print_logical_ast_detailed(shell->ast, 0);
		ft_printf("\n");
	}
	else
		ft_printf("Aucun AST genere\n\n");
	return (0);
}

/**
 * @brief Explains operator precedence
 */
static void	ft_explain_precedence(void)
{
	ft_printf("=== PRECEDENCE DES OPERATEURS ===\n");
	ft_printf("1. | (PIPE) - Priorite la plus haute\n");
	ft_printf("2. && (AND) - Priorite moyenne\n");
	ft_printf("3. || (OR) - Priorite la plus basse\n");
	ft_printf("\nEvaluation: gauche vers droite au meme niveau\n");
	ft_printf("Court-circuit:\n");
	ft_printf("  - && : arrete si commande gauche echoue\n");
	ft_printf("  - || : arrete si commande gauche reussit\n\n");
}

/**
 * @brief Main function for testing logical operators parsing
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @param env Environment variables
 * @return int Exit code
 */
int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	if (argc != 2)
	{
		ft_printf("Usage: %s \"commande avec operateurs logiques\"\n", argv[0]);
		ft_printf("Exemples:\n");
		ft_printf("  %s \"echo hello && pwd\"\n", argv[0]);
		ft_printf("  %s \"false || echo backup\"\n", argv[0]);
		ft_printf("  %s \"ls | grep test && echo found\"\n", argv[0]);
		return (1);
	}
	if (ft_setup(&shell, env) == -1)
	{
		ft_printerr("Erreur: Initialisation echouee\n");
		return (1);
	}
	shell.current_line = argv[1];
	ft_printf("=== TEST DU PARSING DES OPERATEURS LOGIQUES ===\n");
	ft_printf("Commande: \"%s\"\n\n", shell.current_line);
	ft_explain_precedence();
	shell.token = ft_lexing(&shell);
	if (!shell.token)
	{
		ft_printerr("Erreur: Echec du lexing\n");
		ft_cleanup(&shell, 1);
		return (1);
	}
	ft_count_logical_operators(&shell);
	if (!ft_validate_logical_syntax(&shell))
	{
		ft_cleanup(&shell, 1);
		return (1);
	}
	if (ft_test_logical_parsing(&shell) != 0)
	{
		ft_cleanup(&shell, 1);
		return (1);
	}
	ft_printf("=== TEST TERMINE AVEC SUCCES ===\n");
	ft_cleanup(&shell, 0);
	return (0);
}
