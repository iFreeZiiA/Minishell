/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Updates the parentheses depth counter based on token type
 * 
 * This helper function increments the depth counter for opening parentheses
 * and decrements it for closing parentheses, used for tracking nesting levels.
 * 
 * @param token Current token to process
 * @param depth Pointer to the depth counter to update
 */
static void	ft_update_paren_depth(t_token *token, int *depth)
{
	if (token->type == TOKEN_PAREN_OPEN)
		(*depth)++;
	else if (token->type == TOKEN_PAREN_CLOSE)
		(*depth)--;
}

/**
 * @brief Finds the closing parenthesis that matches an opening parenthesis
 * 
 * This function traverses forward from an opening parenthesis to find its
 * corresponding closing parenthesis, properly handling nested parentheses
 * by tracking depth levels.
 * 
 * @param start Token containing the opening parenthesis
 * @return t_list* Token containing the matching closing parenthesis, or NULL
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
 * @brief Finds the opening parenthesis that matches a closing parenthesis
 * 
 * This function traverses backward from a closing parenthesis to find its
 * corresponding opening parenthesis, properly handling nested parentheses
 * by tracking depth levels in reverse.
 * 
 * @param start Starting boundary for the backward search
 * @param end Token containing the closing parenthesis
 * @return t_list* Token containing the matching opening parenthesis, or NULL
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
 * @brief Skips over an entire parentheses group to the next token
 * 
 * This function finds the matching closing parenthesis for an opening
 * parenthesis and returns the token immediately following the closing
 * parenthesis, effectively skipping the entire parenthesized group.
 * 
 * @param start Token containing the opening parenthesis
 * @return t_list* Token immediately after the closing parenthesis, or NULL
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
 * @brief Calculates the parentheses depth level at a specific position
 * 
 * This function traverses from the beginning of the token list to a target
 * position, counting the nesting level of parentheses at that point. This
 * is useful for determining operator precedence and grouping contexts.
 * 
 * @param token_h Head of the token list
 * @param target Target position to calculate depth for
 * @return int Depth level at target position, or -1 on error
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
