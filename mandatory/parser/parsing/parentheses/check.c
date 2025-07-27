/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Counts the number of opening parentheses in the token list
 * 
 * This function traverses the token list and counts all instances
 * of opening parenthesis tokens for validation purposes.
 * 
 * @param token_h Head of the token list to analyze
 * @return int Total number of opening parentheses found
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
 * @brief Counts the number of closing parentheses in the token list
 * 
 * This function traverses the token list and counts all instances
 * of closing parenthesis tokens for validation purposes.
 * 
 * @param token_h Head of the token list to analyze
 * @return int Total number of closing parentheses found
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
 * @brief Validates that parentheses are properly nested and balanced
 * 
 * This function checks the nesting structure of parentheses by maintaining
 * a depth counter. It ensures that:
 * - No closing parenthesis appears without a matching opening one
 * - All opening parentheses have corresponding closing ones
 * - The final depth is zero (balanced)
 * 
 * @param token_h Head of the token list to validate
 * @return bool true if parentheses are properly nested, false otherwise
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
 * @brief Checks for empty parentheses pairs that contain no content
 * 
 * This function validates that parentheses pairs are not empty by
 * looking for consecutive opening and closing parentheses without
 * any meaningful content between them.
 * 
 * @param token_h Head of the token list to check
 * @return bool true if no empty parentheses pairs found, false otherwise
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
 * @brief Performs comprehensive parentheses syntax validation
 * 
 * This function combines all parentheses validation checks:
 * 1. Equal count of opening and closing parentheses
 * 2. Proper nesting without premature closing
 * 3. No empty parentheses pairs
 * 
 * It serves as the main validation entry point for parentheses syntax.
 * 
 * @param token_h Head of the token list to validate
 * @return bool true if all parentheses syntax is valid, false otherwise
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
