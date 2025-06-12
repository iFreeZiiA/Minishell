/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 10:24:31 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Increments depth counter for parentheses tracking
 * 
 * @param token Current token
 * @param depth Pointer to depth counter
 */
static void	ft_update_paren_depth(t_token *token, int *depth)
{
	if (token->type == TOKEN_PAREN_OPEN)
		(*depth)++;
	else if (token->type == TOKEN_PAREN_CLOSE)
		(*depth)--;
}

/**
 * @brief Finds matching closing parenthesis
 * 
 * @param start Token with opening parenthesis
 * @return t_list* Token with matching closing parenthesis
 */
t_list	*ft_find_matching_paren(t_list *start)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	if (!start)
		return (NULL);
	token = start->content.token;
	if (token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	depth = 1;
	curr = start->next;
	while (curr && depth > 0)
	{
		token = curr->content.token;
		ft_update_paren_depth(token, &depth);
		if (depth == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Finds matching opening parenthesis
 * 
 * @param start Starting point for search
 * @param end Token with closing parenthesis
 * @return t_list* Token with matching opening parenthesis
 */
t_list	*ft_find_matching_open_paren(t_list *start, t_list *end)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	if (!end)
		return (NULL);
	token = end->content.token;
	if (token->type != TOKEN_PAREN_CLOSE)
		return (NULL);
	depth = 1;
	curr = end->prev;
	while (curr && curr != start && depth > 0)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_CLOSE)
			depth++;
		else if (token->type == TOKEN_PAREN_OPEN)
			depth--;
		if (depth == 0)
			return (curr);
		curr = curr->prev;
	}
	return (NULL);
}

/**
 * @brief Skips over a parentheses group
 * 
 * @param start Opening parenthesis
 * @return t_list* Token after the closing parenthesis
 */
t_list	*ft_skip_parentheses_group(t_list *start)
{
	t_list	*closing;

	if (!start)
		return (NULL);
	closing = ft_find_matching_paren(start);
	if (!closing)
		return (NULL);
	return (closing->next);
}

/**
 * @brief Gets parentheses depth at specific position
 * 
 * @param token_h Token list head
 * @param target Target position
 * @return int Depth level at target position
 */
int	ft_get_parentheses_depth(t_list *token_h, t_list *target)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	if (!token_h || !target)
		return (-1);
	depth = 0;
	curr = token_h;
	while (curr && curr != target)
	{
		token = curr->content.token;
		ft_update_paren_depth(token, &depth);
		curr = curr->next;
	}
	return (depth);
}
