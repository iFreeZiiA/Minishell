/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:55:09 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Checks if token is a logical operator
 * 
 * @param token Token to check
 * @return bool true if logical operator
 */
bool	ft_is_logical_operator_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->type == TOKEN_AND)
		return (true);
	if (token->type == TOKEN_OR)
		return (true);
	return (false);
}

/**
 * @brief Gets operator precedence for parsing
 * 
 * @param type Token type
 * @return int Precedence level (higher = more priority)
 */
int	ft_get_operator_precedence(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (3);
	if (type == TOKEN_AND)
		return (2);
	if (type == TOKEN_OR)
		return (1);
	return (0);
}

/**
 * @brief Validates logical operator syntax
 * 
 * @param token_h Head of token list
 * @return bool true if syntax is valid
 */
bool	ft_validate_logical_syntax(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	t_token	*prev_token;

	if (!token_h)
		return (false);
	curr = token_h;
	prev_token = NULL;
	while (curr)
	{
		token = curr->content.token;
		if (ft_is_logical_operator_token(token))
		{
			if (!prev_token || ft_is_logical_operator_token(prev_token))
				return (false);
			if (!curr->next)
				return (false);
		}
		prev_token = token;
		curr = curr->next;
	}
	return (true);
}

/**
 * @brief Main logical parsing entry point
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
int	ft_parse_logical_operators(t_shell *shell)
{
	t_list		*last_token;
	t_ast_node	*ast;

	if (!shell || !shell->token)
		return (-1);
	if (!ft_validate_logical_syntax(shell->token))
		return (-1);
	last_token = ft_lstlast(shell->token);
	if (!last_token)
		return (-1);
	ast = ft_parse_logical_expression(shell, shell->token, last_token);
	if (!ast)
		return (-1);
	shell->ast = ast;
	return (0);
}
