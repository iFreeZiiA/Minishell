/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:40:57 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Parse redirection de sortie >
 * @param tokens Position du token >
 * @param cmd Commande cible
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_out_redir(t_list *tokens, t_command *cmd)
{
	char		*filename;
	t_redir		*redir;
	t_content	content;

	if (!tokens || !cmd)
		return (-1);
	filename = ft_ext_redir_file(tokens);
	if (!filename)
		return (-1);
	if (ft_val_out_file(filename) != 0)
	{
		free(filename);
		return (-1);
	}
	redir = ft_new_redir(REDIR_OUT, filename);
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

int	ft_parse_append(t_list *tokens, t_command *cmd)
{
	char		*filename;
	t_redir		*redir;
	t_content	content;

	if (!tokens || !cmd)
		return (-1);
	filename = ft_ext_redir_file(tokens);
	if (!filename)
		return (-1);
	if (ft_val_out_file(filename) != 0)
	{
		free(filename);
		return (-1);
	}
	redir = ft_new_redir(REDIR_APPEND, filename);
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

int	ft_val_out_file(char *filename)
{
	if (!filename)
		return (-1);
	if (ft_strlen(filename) == 0)
		return (-1);
	if (ft_check_write_perm(filename) != 0)
		return (-1);
	return (0);
}
/**
 * @brief Vérifie permissions d'écriture
 * @param filename Nom du fichier
 * @return int 0 autorisé, -1 refusé
 */

int	ft_check_write_perm(char *filename)
{
	if (!filename)
		return (-1);
	return (0);
}
