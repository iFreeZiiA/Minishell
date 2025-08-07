/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:55:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Traite tous les heredocs d'une commande
 * @param cmd Commande contenant les redirections
 * @return int 0 succès, -1 erreur
 */
int	ft_process_heredocs(t_command *cmd)
{
	t_list	*current;
	t_redir	*redir;
	int		heredoc_fd;

	if (!cmd || !cmd->redirs)
		return (0);
	current = cmd->redirs;
	while (current)
	{
		if (current->type == TYPE_REDIR)
		{
			redir = current->content.redir;
			if (redir && redir->type == REDIR_HEREDOC)
			{
				heredoc_fd = ft_handle_heredoc(redir);
				if (heredoc_fd == -1)
					return (-1);
				redir->fd = heredoc_fd;
			}
		}
		current = current->next;
	}
	return (0);
}
