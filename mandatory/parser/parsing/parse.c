/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 17:01:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Checks if token list contains logical operators
 * 
 * @param tokens Token list head
 * @return bool true if logical operators found
 */
static bool	ft_contains_logical_ops(t_list *tokens)
{
	t_list	*curr;
	t_token	*token;

	if (!tokens)
		return (false);
	curr = tokens;
	while (curr)
	{
		token = curr->content.token;
		if (ft_is_logical_operator_token(token) || token->type == TOKEN_PIPE)
			return (true);
		curr = curr->next;
	}
	return (false);
}

/**
 * @brief Validates parsing prerequisites
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
static int	ft_validate_parse_input(t_shell *shell)
{
	if (!shell)
		return (-1);
	if (!shell->token)
		return (-1);
	if (ft_lexer_checker(shell) != 0)
		return (-1);
	return (0);
}

/**
 * @brief Performs syntax validation on tokens
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
static int	ft_syntax_validation(t_shell *shell)
{
	if (!ft_validate_logical_syntax(shell->token))
		return (-1);
	return (0);
}

/**
 * @brief Main parsing entry point integrating all phases
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
int	ft_parse(t_shell *shell)
{
	if (ft_validate_parse_input(shell) != 0)
		return (-1);
	if (ft_syntax_validation(shell) != 0)
		return (-1);
	if (ft_contains_logical_ops(shell->token))
	{
		if (ft_parse_logical_operators(shell) != 0)
			return (-1);
	}
	else
	{
		shell->ast = ft_parser(shell->token, shell);
		if (!shell->ast)
			return (-1);
	}
	return (0);
}
