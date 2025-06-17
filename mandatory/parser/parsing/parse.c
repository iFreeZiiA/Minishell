/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 19:07:29 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Validates basic syntax rules for token sequence
 * 
 * This function checks if the token sequence follows basic shell syntax rules:
 * - Must not be empty
 * - Cannot start or end with an operator
 * - Cannot have consecutive operators
 * - Must contain at least one operand (word token)
 * 
 * @param tokens Linked list of tokens to validate
 * @return int 0 on valid syntax, -1 on syntax error
 */
static int	ft_validate_basic_syntax(t_list *tokens)
{
	t_list	*curr;
	bool	expect_operand;
	bool	found_operand;

	if (!tokens)
		return (-1);
	curr = tokens;
	expect_operand = true;
	found_operand = false;
	while (curr)
	{
		if (ft_is_operator_token(curr->content.token))
		{
			if (expect_operand || !curr->next)
				return (-1);
			expect_operand = true;
		}
		else if (ft_is_word_token(curr->content.token))
		{
			expect_operand = false;
			found_operand = true;
		}
		curr = curr->next;
	}
	if (!found_operand)
		return (-1);
	return (0);
}

/**
 * @brief Preprocesses tokens before AST construction
 * 
 * This function performs validation checks on the token list before
 * attempting to build the Abstract Syntax Tree. It validates:
 * - Basic syntax rules (operators, operands placement)
 * - Parentheses matching and balance
 * 
 * @param shell Pointer to shell structure containing tokens
 * @return int 0 on success, -1 on validation error
 */
static int	ft_preprocess_tokens(t_shell *shell)
{
	if (ft_validate_basic_syntax(shell->token) < 0)
	{
		ft_printerr("minishell: syntax error\n");
		return (-1);
	}
	if (!ft_validate_parentheses(shell->token))
	{
		ft_printerr("minishell: unmatched parentheses\n");
		return (-1);
	}
	return (0);
}

/**
 * @brief Builds the Abstract Syntax Tree from preprocessed tokens
 * 
 * This function creates an AST by parsing the token list into a tree structure
 * that represents the command structure with proper operator precedence.
 * 
 * @param shell Pointer to shell structure containing tokens
 * @return t_ast_node* Pointer to root of AST, or NULL on failure
 */
static t_ast_node	*ft_build_ast(t_shell *shell)
{
	t_ast_node	*ast;

	ast = ft_parse_expression(shell, shell->token, shell->token, NULL);
	return (ast);
}

/**
 * @brief Main parsing function that coordinates the parsing process
 * 
 * This function orchestrates the complete parsing process:
 * 1. Validates input parameters (shell and tokens)
 * 2. Preprocesses tokens for syntax validation
 * 3. Builds the Abstract Syntax Tree from tokens
 * 4. Prints the AST structure for debugging
 * 
 * @param shell Pointer to shell structure containing tokens and AST
 * @return int 0 on successful parsing, -1 on error
 */
int	ft_parse(t_shell *shell)
{
	if (!shell || !shell->token)
	// {
		// ft_printerr("FT_PARSE0\n");
		return (-1);
	// }
	if (ft_preprocess_tokens(shell) < 0)
	// {
		// ft_printerr("FT_PARSE1\n");
		return (-1);
	// }
	shell->ast = ft_build_ast(shell);
	if (!shell->ast)
	// {
		// ft_printerr("FT_PARSE2\n");
		return (-1);
	// }
	ft_print_ast_tree(shell->ast, 0);
	return (0);
}
