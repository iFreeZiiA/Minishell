/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_find.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:31:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Trouve la première parenthèse ouvrante dans la liste
 * @param token_list Liste de tokens
 * @return t_list* Token de la parenthèse ouvrante ou NULL
 */
t_list	*ft_find_first_open_paren(t_list *token_list)
{
	t_list	*current;
	t_token	*token;

	if (!token_list)
		return (NULL);
	current = token_list;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (token->type == TOKEN_PAREN_OPEN)
				return (current);
		}
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Trouve la parenthèse fermante correspondant à l'ouvrante
 * @param start Token de la parenthèse ouvrante
 * @return t_list* Token de la parenthèse fermante correspondante ou NULL
 */
t_list	*ft_find_matching_paren(t_list *start)
{
	t_list	*current;
	t_token	*token;
	int		balance;

	if (!start || start->type != TYPE_TOKEN)
		return (NULL);
	if (start->content.token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	balance = 1;
	current = start->next;
	while (current && balance > 0)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (token->type == TOKEN_PAREN_OPEN)
				balance++;
			else if (token->type == TOKEN_PAREN_CLOSE)
				balance--;
		}
		if (balance == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Trouve la dernière parenthèse fermante dans la liste
 * @param token_list Liste de tokens
 * @return t_list* Token de la dernière parenthèse fermante ou NULL
 */
t_list	*ft_find_last_close_paren(t_list *token_list)
{
	t_list	*current;
	t_list	*last_close;
	t_token	*token;

	if (!token_list)
		return (NULL);
	last_close = NULL;
	current = token_list;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (token->type == TOKEN_PAREN_CLOSE)
				last_close = current;
		}
		current = current->next;
	}
	return (last_close);
}
