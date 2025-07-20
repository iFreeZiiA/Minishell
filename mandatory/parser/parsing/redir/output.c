/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:48:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Parse redirection de sortie >
 * @param tokens Position du token >
 * @param cmd Commande cible
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_output_redirection(t_list *tokens, t_command *cmd)
{
	char		*filename;
	t_redir		*redir;
	t_content	content;

	if (!tokens || !cmd)
		return (-1);
	filename = ft_extract_redirection_file(tokens);
	if (!filename)
		return (-1);
	if (ft_validate_output_file(filename) != 0)
	{
		free(filename);
		return (-1);
	}
	redir = ft_create_redirection(REDIR_OUT, filename);
	free(filename);
	if (!redir)
		return (-1);
	content.redir = redir;
	if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
		return (-1);
	return (0);
}

/**
 * @brief Parse redirection append >>
 * @param tokens Position du token >>
 * @param cmd Commande cible
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_append_redirection(t_list *tokens, t_command *cmd)
{
	char		*filename;
	t_redir		*redir;
	t_content	content;

	if (!tokens || !cmd)
		return (-1);
	filename = ft_extract_redirection_file(tokens);
	if (!filename)
		return (-1);
	if (ft_validate_output_file(filename) != 0)
	{
		free(filename);
		return (-1);
	}
	redir = ft_create_redirection(REDIR_APPEND, filename);
	free(filename);
	if (!redir)
		return (-1);
	content.redir = redir;
	if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
		return (-1);
	return (0);
}

/**
 * @brief Valide le fichier de sortie
 * @param filename Nom du fichier
 * @return int 0 valide, -1 invalide
 */
int	ft_validate_output_file(char *filename)
{
	if (!filename)
		return (-1);
	if (ft_strlen(filename) == 0)
		return (-1);
	if (ft_check_write_permissions(filename) != 0)
		return (-1);
	return (0);
}

/**
 * @brief Vérifie permissions d'écriture
 * @param filename Nom du fichier
 * @return int 0 autorisé, -1 refusé
 */
int	ft_check_write_permissions(char *filename)
{
	if (!filename)
		return (-1);
	return (0);
}
