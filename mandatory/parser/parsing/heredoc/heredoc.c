/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 09:25:34 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Lit une ligne du heredoc depuis stdin
 * @param delimiter Délimiteur de fin
 * @return char* Ligne lue ou NULL si délimiteur trouvé
 */
char	*ft_read_heredoc_line(char *delimiter)
{
	char	*line;
	int		len;

	ft_putstr_fd("> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (NULL);
	}
	line[len - 1] = '\n';
	return (line);
}

/**
 * @brief Écrit le contenu dans le pipe
 * @param content Contenu à écrire
 * @param write_fd Descripteur d'écriture
 * @return int 0 succès, -1 erreur
 */
int	ft_write_to_pipe(char *content, int write_fd)
{
	int	len;
	int	written;

	if (!content)
		return (0);
	len = ft_strlen(content);
	written = write(write_fd, content, len);
	if (written != len)
		return (-1);
	return (0);
}

/**
 * @brief Accumule les lignes du heredoc
 * @param content Contenu existant
 * @param line Nouvelle ligne
 * @return char* Nouveau contenu
 */
char	*ft_append_line(char *content, char *line)
{
	char	*new_content;

	if (!line)
		return (content);
	if (!content)
		return (ft_strdup(line));
	new_content = ft_strjoin(content, line);
	free(content);
	return (new_content);
}

/**
 * @brief Traite un heredoc complet
 * @param redir Redirection heredoc
 * @return int 0 succès, -1 erreur
 */
int	ft_handle_heredoc(t_redir *redir)
{
	char	*content;
	char	*line;
	int		pipefd[2];

	if (!redir || redir->type != REDIR_HEREDOC)
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	content = NULL;
	while (1)
	{
		line = ft_read_heredoc_line(redir->file);
		if (!line)
			break ;
		content = ft_append_line(content, line);
		free(line);
		if (!content)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			return (-1);
		}
	}
	if (ft_write_to_pipe(content, pipefd[1]) != 0)
	{
		free(content);
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	free(content);
	close(pipefd[1]);
	redir->fd = pipefd[0];
	return (0);
}

/**
 * @brief Traite tous les heredocs d'une commande
 * @param cmd Commande contenant les redirections
 * @return int 0 succès, -1 erreur
 */
int	ft_process_heredocs(t_command *cmd)
{
	t_list	*current;

	if (!cmd || !cmd->redirs)
		return (0);
	current = cmd->redirs;
	while (current)
	{
		if (current->type == TYPE_REDIR && current->content.redir)
		{
			if (current->content.redir->type == REDIR_HEREDOC)
			{
				if (ft_handle_heredoc(current->content.redir) != 0)
					return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}
