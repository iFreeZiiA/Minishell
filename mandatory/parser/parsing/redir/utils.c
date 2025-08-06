/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:40:57 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Vérifie si un token est une redirection
 * @param token Token à vérifier
 * @return int 1 si redirection, 0 sinon
 */
int	ft_is_redir_tok(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC);
}

/**
 * @brief Extrait le nom de fichier après une redirection
 * @param redir_token Token de redirection
 * @return char* Nom du fichier ou NULL
 */
char	*ft_ext_redir_file(t_list *redir_token)
{
	t_list	*next_token;

	if (!redir_token)
		return (NULL);
	next_token = redir_token->next;
	if (!next_token || !next_token->content.token)
		return (NULL);
	if (next_token->content.token->type != TOKEN_WORD)
		return (NULL);
	return (ft_strdup(next_token->content.token->value));
}

/**
 * @brief Vérifie si une redirection est de type sortie
 * @param redir Structure redirection
 * @return int 1 si sortie, 0 sinon
 */
int	ft_is_out_redir(t_redir *redir)
{
	if (!redir)
		return (0);
	return (redir->type == REDIR_OUT || redir->type == REDIR_APPEND);
}

/**
 * @brief Crée une structure redirection
 * @param type Type de redirection
 * @param filename Nom du fichier
 * @return t_redir* Nouvelle redirection
 */
t_redir	*ft_new_redir(redir_type type, char *filename)
{
	t_redir	*redir;

	if (!filename)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(filename);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->fd = -1;
	return (redir);
}
