/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 10:24:53 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Counts opening parentheses in token list
 * 
 * @param token_h Token list head
 * @return int Number of opening parentheses
 */
static int	ft_count_open_parens(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	int		count;

	count = 0;
	curr = token_h;
	while (curr)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			count++;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Counts closing parentheses in token list
 * 
 * @param token_h Token list head
 * @return int Number of closing parentheses
 */
static int	ft_count_close_parens(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	int		count;

	count = 0;
	curr = token_h;
	while (curr)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_CLOSE)
			count++;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Checks if parentheses are properly nested
 * 
 * @param token_h Token list head
 * @return bool true if properly nested, false otherwise
 */
static bool	ft_check_parentheses_nesting(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	depth = 0;
	curr = token_h;
	while (curr)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			depth++;
		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			depth--;
			if (depth < 0)
				return (false);
		}
		curr = curr->next;
	}
	return (depth == 0);
}

/**
 * @brief Checks for empty parentheses pairs
 * 
 * @param token_h Token list head
 * @return bool true if no empty pairs, false otherwise
 */
static bool	ft_check_empty_parentheses(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	t_token	*next_token;

	curr = token_h;
	while (curr && curr->next)
	{
		token = curr->content.token;
		next_token = curr->next->content.token;
		if (token->type == TOKEN_PAREN_OPEN &&
			next_token->type == TOKEN_PAREN_CLOSE)
			return (false);
		curr = curr->next;
	}
	return (true);
}

/**
 * @brief Comprehensive parentheses syntax check
 * 
 * @param token_h Token list head
 * @return bool true if syntax is valid, false otherwise
 */
bool	ft_check_parentheses_syntax(t_list *token_h)
{
	int	open_count;
	int	close_count;

	if (!token_h)
		return (true);
	open_count = ft_count_open_parens(token_h);
	close_count = ft_count_close_parens(token_h);
	if (open_count != close_count)
		return (false);
	if (!ft_check_parentheses_nesting(token_h))
		return (false);
	if (!ft_check_empty_parentheses(token_h))
		return (false);
	return (true);
}
