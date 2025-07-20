/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:48:53 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Parse redirection d'entrée <
 * @param tokens Position du token <
 * @param cmd Commande cible
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_input_redirection(t_list *tokens, t_command *cmd)
{
	char		*filename;
	t_redir		*redir;
	t_content	content;

	if (!tokens || !cmd)
		return (-1);
	filename = ft_extract_redirection_file(tokens);
	if (!filename)
		return (-1);
	if (ft_validate_input_file(filename) != 0)
	{
		free(filename);
		return (-1);
	}
	redir = ft_create_redirection(REDIR_IN, filename);
	free(filename);
	if (!redir)
		return (-1);
	content.redir = redir;
	if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
		return (-1);
	return (0);
}

/**
 * @brief Parse heredoc <<
 * @param tokens Position du token <<
 * @param cmd Commande cible
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_heredoc_redirection(t_list *tokens, t_command *cmd)
{
	char		*delimiter;
	t_redir		*redir;
	t_content	content;

	if (!tokens || !cmd)
		return (-1);
	delimiter = ft_extract_heredoc_delimiter(tokens);
	if (!delimiter)
		return (-1);
	redir = ft_create_redirection(REDIR_HEREDOC, delimiter);
	free(delimiter);
	if (!redir)
		return (-1);
	content.redir = redir;
	if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
		return (-1);
	return (0);
}

/**
 * @brief Valide le fichier d'entrée
 * @param filename Nom du fichier
 * @return int 0 valide, -1 invalide
 */
int	ft_validate_input_file(char *filename)
{
	if (!filename)
		return (-1);
	if (ft_strlen(filename) == 0)
		return (-1);
	return (0);
}

/**
 * @brief Extrait le délimiteur heredoc
 * @param tokens Liste de tokens
 * @return char* Délimiteur ou NULL
 */
char	*ft_extract_heredoc_delimiter(t_list *tokens)
{
	t_list	*next_token;

	if (!tokens)
		return (NULL);
	next_token = tokens->next;
	if (!next_token || !next_token->content.token)
		return (NULL);
	if (next_token->content.token->type != TOKEN_WORD)
		return (NULL);
	return (ft_strdup(next_token->content.token->value));
}
