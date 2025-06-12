/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   depth.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 10:25:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Updates depth based on token type
 * 
 * @param token Current token
 * @param depth Current depth
 * @return int Updated depth
 */
static int	ft_update_depth(t_token *token, int depth)
{
	if (token->type == TOKEN_PAREN_OPEN)
		return (depth + 1);
	if (token->type == TOKEN_PAREN_CLOSE)
		return (depth - 1);
	return (depth);
}

/**
 * @brief Finds maximum parentheses depth in token list
 * 
 * @param token_h Token list head
 * @return int Maximum depth reached
 */
int	ft_find_max_parentheses_depth(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	int		depth;
	int		max_depth;

	if (!token_h)
		return (0);
	depth = 0;
	max_depth = 0;
	curr = token_h;
	while (curr)
	{
		token = curr->content.token;
		depth = ft_update_depth(token, depth);
		if (depth > max_depth)
			max_depth = depth;
		curr = curr->next;
	}
	return (max_depth);
}

/**
 * @brief Checks if token is at specific depth level
 * 
 * @param token_h Token list head
 * @param target Target token
 * @param level Target depth level
 * @return bool true if at level, false otherwise
 */
bool	ft_is_token_at_depth_level(t_list *token_h, t_list *target, int level)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	if (!token_h || !target)
		return (false);
	depth = 0;
	curr = token_h;
	while (curr && curr != target)
	{
		token = curr->content.token;
		depth = ft_update_depth(token, depth);
		curr = curr->next;
	}
	return (depth == level);
}

/**
 * @brief Finds tokens at specific depth level
 * 
 * @param token_h Token list head
 * @param level Target depth level
 * @return t_list* First token at specified level
 */
t_list	*ft_find_token_at_depth(t_list *token_h, int level)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	if (!token_h)
		return (NULL);
	depth = 0;
	curr = token_h;
	while (curr)
	{
		token = curr->content.token;
		if (depth == level)
			return (curr);
		depth = ft_update_depth(token, depth);
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Checks if all parentheses are at valid depths
 * 
 * @param token_h Token list head
 * @return bool true if valid depths, false otherwise
 */
bool	ft_validate_parentheses_depth(t_list *token_h)
{
	t_list	*curr;
	t_token	*token;
	int		depth;

	if (!token_h)
		return (true);
	depth = 0;
	curr = token_h;
	while (curr)
	{
		token = curr->content.token;
		depth = ft_update_depth(token, depth);
		if (depth < 0)
			return (false);
		curr = curr->next;
	}
	return (depth == 0);
}
