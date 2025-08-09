/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:57:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:01:13 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Vérifie si un token nécessite une expansion wildcard
 * @param token Token à tester
 * @return int 1 si expansion nécessaire, 0 sinon
 */
static int	ft_needs_wildcard_expansion(t_token *token)
{
	int	i;

	if (!token || token->type != TOKEN_WORD || !token->value)
		return (0);
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

/* Fonctions utilitaires dans wildcard_utils.c */

/**
 * @brief Connecte les nouveaux tokens à la liste
 */
static void	ft_connect_new_tokens(t_list **token_list, t_list *current,
		t_list *new_tokens)
{
	t_list	*last_new;
	t_list	*next_token;

	last_new = ft_lstlast(new_tokens);
	next_token = current->next;
	if (current == *token_list)
		*token_list = new_tokens;
	else
	{
		current->prev->next = new_tokens;
		new_tokens->prev = current->prev;
	}
	last_new->next = next_token;
	if (next_token)
		next_token->prev = last_new;
}

/**
 * @brief Remplace un token wildcard par ses matches
 * @param token_list Pointeur vers la liste de tokens
 * @param current Token courant à remplacer
 * @return int 0 succès, -1 erreur
 */
static int	ft_replace_wildcard_token(t_list **token_list, t_list *current)
{
	char	**matches;
	t_list	*new_tokens;

	matches = ft_get_matching_files(current->content.token->value);
	if (!matches)
		return (-1);
	if (!matches[0])
	{
		ft_free_string_array(matches);
		return (0);
	}
	new_tokens = ft_create_match_tokens(matches);
	ft_free_string_array(matches);
	if (!new_tokens)
		return (-1);
	ft_connect_new_tokens(token_list, current, new_tokens);
	ft_free_token(current->content.token);
	free(current);
	return (0);
}
