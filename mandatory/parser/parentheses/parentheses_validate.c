/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_validate.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Vérifie si un token est une parenthèse ouvrante
 * @param token Token à vérifier
 * @return int 1 si parenthèse ouvrante, 0 sinon
 */
static int	ft_is_open_paren(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PAREN_OPEN);
}

/**
 * @brief Vérifie si un token est une parenthèse fermante
 * @param token Token à vérifier
 * @return int 1 si parenthèse fermante, 0 sinon
 */
static int	ft_is_close_paren(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PAREN_CLOSE);
}

/**
 * @brief Vérifie si la liste de tokens contient des parenthèses
 * @param token_list Liste de tokens
 * @return int 1 si contient des parenthèses, 0 sinon
 */
int	ft_has_parentheses(t_list *token_list)
{
	t_list	*current;
	t_token	*token;

	if (!token_list)
		return (0);
	current = token_list;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (ft_is_open_paren(token) || ft_is_close_paren(token))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

/**
 * @brief Valide l'équilibre des parenthèses dans une liste de tokens
 * @param token_list Liste de tokens à valider
 * @return int 0 si équilibrées, -1 si erreur
 */
int	ft_validate_parentheses_balance(t_list *token_list)
{
	t_list	*current;
	t_token	*token;
	int		balance;

	if (!token_list)
		return (0);
	balance = 0;
	current = token_list;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (ft_is_open_paren(token))
				balance++;
			else if (ft_is_close_paren(token))
				balance--;
			if (balance < 0)
				return (-1);
		}
		current = current->next;
	}
	if (balance != 0)
		return (-1);
	return (0);
}
