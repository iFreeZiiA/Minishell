/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:45:00 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 16:08:19 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Compte les tokens WORD qui ne sont PAS des fichiers de redirection
 * @param tokens Liste de tokens
 * @return int Nombre de vrais arguments de commande
 */
int	ft_count_command_args_only(t_list *tokens)
{
	t_list	*curr;
	t_list	*prev;
	int		count;

	count = 0;
	curr = tokens;
	prev = NULL;
	while (curr)
	{
		if (curr->content.token->type == TOKEN_WORD)
		{
			if (!ft_is_redirection_file(curr, prev))
				count++;
		}
		prev = curr;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Traite un token pour l'extraction d'arguments
 * @param curr Token actuel
 * @param prev Token précédent
 * @param args Tableau d'arguments
 * @param i Index actuel
 * @return int Nouvel index
 */
static int	ft_process_token_for_args(t_list *curr, t_list *prev,
			char **args, int i)
{
	if (curr->content.token->type == TOKEN_WORD)
	{
		if (!ft_is_redirection_file(curr, prev))
		{
			args[i] = ft_strdup(curr->content.token->value);
			return (i + 1);
		}
	}
	return (i);
}

/**
 * @brief Extrait seulement les arguments de commande
 * @param tokens Liste de tokens
 * @param count Nombre d'arguments attendus
 * @return char** Tableau d'arguments sans les fichiers de redirection
 */
char	**ft_extract_command_args_only(t_list *tokens, int count)
{
	char	**args;
	t_list	*curr;
	t_list	*prev;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	curr = tokens;
	prev = NULL;
	i = 0;
	while (curr && i < count)
	{
		i = ft_process_token_for_args(curr, prev, args, i);
		prev = curr;
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}
