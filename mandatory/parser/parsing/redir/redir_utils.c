/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 21:17:07 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Vérifie si un token est une redirection
 * @param token Token à vérifier
 * @return int 1 si redirection, 0 sinon
 */
int	ft_is_redirection_token(t_token *token)
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
char	*ft_extract_redirection_file(t_list *redir_token)
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
int	ft_is_output_redirection(t_redir *redir)
{
	if (!redir)
		return (0);
	return (redir->type == REDIR_OUT || redir->type == REDIR_APPEND);
}
