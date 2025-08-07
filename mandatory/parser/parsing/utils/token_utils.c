/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 22:39:22 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_count_word_tokens(t_list *tokens)
{
	t_list	*curr;
	int		count;

	curr = tokens;
	count = 0;
	while (curr)
	{
		if (curr->content.token->type == TOKEN_WORD)
			count++;
		curr = curr->next;
	}
	return (count);
}

char	**ft_extract_command_args(t_list *tokens, int count)
{
	char	**args;
	t_list	*curr;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	curr = tokens;
	i = 0;
	while (curr && i < count)
	{
		if (curr->content.token->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(curr->content.token->value);
			i++;
		}
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}

int	ft_count_word_tokens_until_pipe(t_list *tokens)
{
	t_list	*curr;
	int		count;

	curr = tokens;
	count = 0;
	while (curr && curr->content.token->type != TOKEN_PIPE)
	{
		if (curr->content.token->type == TOKEN_WORD)
			count++;
		curr = curr->next;
	}
	return (count);
}

char	**ft_extract_command_args_until_pipe(t_list *tokens, int count)
{
	char	**args;
	t_list	*curr;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	curr = tokens;
	i = 0;
	while (curr && curr->content.token->type != TOKEN_PIPE && i < count)
	{
		if (curr->content.token->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(curr->content.token->value);
			i++;
		}
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}

/**
 * @brief Vérifie si un token est un fichier de redirection
 * @param current Token actuel
 * @param prev Token précédent
 * @return bool true si c'est un fichier de redirection
 */
bool		ft_is_redirection_file(t_list *current, t_list *prev)
{
	if (!current || !prev || current->content.token->type != TOKEN_WORD)
		return (false);
	
	return (ft_is_redir_tok(prev->content.token));
}

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

	curr = tokens;
	prev = NULL;
	count = 0;
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
 * @brief Extrait seulement les arguments de commande (pas les fichiers de redirection)
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
		if (curr->content.token->type == TOKEN_WORD)
		{
			if (!ft_is_redirection_file(curr, prev))
			{
				args[i] = ft_strdup(curr->content.token->value);
				i++;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}
