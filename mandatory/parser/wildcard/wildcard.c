/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:58:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Vérifie si un token nécessite une expansion wildcard
 * @param token Token à tester
 * @return int 1 si expansion nécessaire, 0 sinon
 */
int	ft_needs_wildcard_expansion(t_token *token)
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

/**
 * @brief Remplace un token wildcard par ses matches dans la liste
 * @param token_list Pointeur vers la liste de tokens
 * @param current Token courant à traiter
 * @return int 0 succès, -1 erreur
 */
int	ft_replace_wildcard_token(t_list **token_list, t_list *current)
{
	char	**matches;
	t_list	*new_tokens;
	t_list	*last_new;
	t_list	*next_token;

	if (!ft_needs_wildcard_expansion(current->content.token))
		return (0);
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
	ft_free_token(current->content.token);
	free(current);
	return (0);
}

/**
 * @brief Traite tous les wildcards dans une liste de tokens
 * @param shell Structure shell
 * @param token_list Pointeur vers la liste de tokens
 */
void	ft_expand_wildcards(t_shell *shell, t_list **token_list)
{
	t_list	*current;
	t_list	*next;

	if (!shell || !token_list || !*token_list)
		return ;
	current = *token_list;
	while (current)
	{
		next = current->next;
		if (current->type == TYPE_TOKEN
			&& ft_needs_wildcard_expansion(current->content.token))
		{
			if (ft_replace_wildcard_token(token_list, current) == -1)
				return ;
		}
		current = next;
	}
}
