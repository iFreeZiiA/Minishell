/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Libère un tableau de strings
 * @param array Tableau à libérer
 */
void	ft_free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/**
 * @brief Libère un token
 * @param token Token à libérer
 */
void	ft_free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}

/**
 * @brief Crée un nouveau token à partir d'un nom de fichier
 * @param filename Nom de fichier
 * @return t_token* Nouveau token ou NULL si erreur
 */
t_token	*ft_create_filename_token(char *filename)
{
	t_token	*token;

	if (!filename)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = TOKEN_WORD;
	token->value = ft_strdup(filename);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

/**
 * @brief Crée une liste de tokens à partir des matches
 * @param matches Tableau de noms de fichiers
 * @return t_list* Liste de tokens ou NULL si erreur
 */
t_list	*ft_create_match_tokens(char **matches)
{
	t_list		*token_list;
	t_content	content;
	int			i;

	if (!matches || !matches[0])
		return (NULL);
	token_list = NULL;
	i = 0;
	while (matches[i])
	{
		content.token = ft_create_filename_token(matches[i]);
		if (!content.token)
		{
			ft_lstfree_t(token_list);
			return (NULL);
		}
		ft_lstadd_back(&token_list, content, TYPE_TOKEN);
		i++;
	}
	return (token_list);
}
