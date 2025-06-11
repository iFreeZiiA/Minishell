/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:37:58 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Validates parsing prerequisites
 * 
 * @param shell The shell structure
 * @return int 0 on success, -1 on error
 */
static int	ft_validate_parsing_input(t_shell *shell)
{
	if (!shell || !shell->token)
		return (-1);
	if (ft_lexer_checker(shell) < 0)
		return (-1);
	if (!ft_validate_parentheses(shell->token))
		return (-1);
	return (0);
}

/**
 * @brief Prepares tokens for parsing
 * 
 * @param shell The shell structure
 * @param token_h Pointer to token list head
 * @return int 0 on success, -1 on error
 */
static int	ft_prepare_tokens(t_shell *shell, t_list **token_h)
{
	*token_h = shell->token;
	ft_exp_wildcard(shell, token_h);
	shell->token = *token_h;
	return (0);
}

/**
 * @brief Builds AST from prepared tokens
 * 
 * @param shell The shell structure
 * @param token_h Token list head
 * @return int 0 on success, -1 on error
 */
static int	ft_build_ast(t_shell *shell, t_list *token_h)
{
	shell->ast = ft_parse_expression(shell, token_h, token_h, NULL);
	if (!shell->ast)
		return (-1);
	return (0);
}

/**
 * @brief Main parsing function to build the AST
 * 
 * @param shell The shell structure
 * @return int 0 on success, -1 on error
 */
int	ft_parsing(t_shell *shell)
{
	t_list	*token_h;
	
	if (ft_validate_parsing_input(shell) < 0)
		return (-1);
	if (ft_prepare_tokens(shell, &token_h) < 0)
		return (-1);
	if (ft_build_ast(shell, token_h) < 0)
		return (-1);
	return (0);
}
