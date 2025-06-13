/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 10:23:58 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Checks if parentheses are balanced
 * 
 * @param token_h Token list head
 * @return int 0 if balanced, negative if unbalanced
 */
int	ft_validate_parentheses_balance(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	int		balance;

	if (!token_h)
		return (-1);
	balance = 0;
	curr = token_h;
	while (curr)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			balance++;
		else if (token->type == TOKEN_PAREN_CLOSE)
			balance--;
		if (balance < 0)
			return (-2);
		curr = curr->next;
	}
	if (balance != 0)
		return (-3);
	return (0);
}

/**
 * @brief Validates parentheses order and syntax
 * 
 * @param start Start of token range
 * @param end End of token range
 * @return bool true if valid, false otherwise
 */
bool	ft_validate_parentheses_order(t_list *start, t_list *end)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	if (!start)
		return (false);
	depth = 0;
	curr = start;
	while (curr && curr != end)
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
	return (true);
}

/**
 * @brief Checks if content between parentheses is valid
 * 
 * @param start Opening parenthesis
 * @param end Closing parenthesis
 * @return bool true if valid content, false otherwise
 */
bool	ft_validate_parentheses_content(t_list *start, t_list *end)
{
	t_list	*curr;
	t_token	*token;

	if (!start || !end || start == end)
		return (false);
	curr = start->next;
	if (curr == end)
		return (false);
	while (curr && curr != end)
	{
		token = curr->content.token;
		if (!token)
			return (false);
		curr = curr->next;
	}
	return (true);
}

/**
 * @brief Checks if expression is wrapped in parentheses
 * 
 * @param start Start of expression
 * @param end End of expression
 * @return bool true if wrapped, false otherwise
 */
bool	ft_validate_parentheses_wrapped(t_list *start, t_list *end)
{
	t_token	*first_token;
	t_token	*last_token;
	t_list	*matching;

	if (!start || !end)
		return (false);
	first_token = start->content.token;
	last_token = end->content.token;
	if (first_token->type != TOKEN_PAREN_OPEN)
		return (false);
	if (last_token->type != TOKEN_PAREN_CLOSE)
		return (false);
	matching = ft_find_matching_paren(start);
	if (matching != end)
		return (false);
	return (true);
}

/**
 * @brief Complete parentheses validation
 * 
 * @param token_h Token list head
 * @return bool true if all parentheses are valid, false otherwise
 */
bool	ft_validate_parentheses(t_list *token_h)
{
	int	balance_result;

	if (!token_h)
		return (false);
	balance_result = ft_validate_parentheses_balance(token_h);
	if (balance_result != 0)
		return (false);
	if (!ft_validate_parentheses_order(token_h, NULL))
		return (false);
	return (true);
}
