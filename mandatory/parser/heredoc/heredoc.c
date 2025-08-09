/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:50:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 13:28:19 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"
#include <sys/wait.h>

/**
 * @brief Lit une ligne du heredoc depuis stdin
 * @param delimiter Délimiteur de fin
 * @return char* Ligne lue ou NULL si délimiteur trouvé
 */
char	*ft_read_heredoc_line(char *delimiter)
{
	char	*line;
	size_t	len;

	line = NULL;
	len = 0;
	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		if (getline(&line, &len, stdin) == -1)
			return (NULL);
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
	}
	if (!line)
		return (NULL);
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * @brief Accumule les lignes du heredoc
 * @param content Contenu existant  
 * @param line Nouvelle ligne
 * @return char* Nouveau contenu
 */
char	*ft_append_line(char *content, char *line)
{
	char	*tmp;
	char	*result;

	if (!content)
		return (ft_strjoin(line, "\n"));
	tmp = ft_strjoin(content, line);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, "\n");
	free(tmp);
	return (result);
}

/**
 * @brief Écrit le contenu dans le pipe
 * @param content Contenu à écrire
 * @param write_fd Descripteur d'écriture
 * @return int 0 succès, -1 erreur
 */
int	ft_write_to_pipe(char *content, int write_fd)
{
	if (!content)
		return (0);
	if (write(write_fd, content, ft_strlen(content)) == -1)
		return (-1);
	return (0);
}

/**
 * @brief Traite un heredoc complet - crée un fichier temporaire
 * @param redir Redirection heredoc
 * @return int File descriptor du fichier temporaire, -1 erreur
 */
static int	ft_process_heredoc_content(t_redir *redir, char *temp_path)
{
	int		temp_fd;
	char	*line;

	temp_fd = open(temp_path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (temp_fd == -1)
		return (-1);
	while (1)
	{
		line = ft_read_heredoc_line(redir->file);
		if (!line)
			break ;
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	close(temp_fd);
	return (0);
}

int	ft_handle_heredoc(t_redir *redir)
{
	int		temp_fd;
	char	*temp_filename;
	char	temp_path[256];

	if (!redir || !redir->file)
		return (-1);
	snprintf(temp_path, 256, "/tmp/minishell_heredoc_%d_%p",
		getpid(), (void*)redir);
	if (ft_process_heredoc_content(redir, temp_path) == -1)
		return (-1);
	temp_fd = open(temp_path, O_RDONLY);
	if (temp_fd == -1)
		return (-1);
	temp_filename = ft_strdup(temp_path);
	if (temp_filename)
	{
		free(redir->file);
		redir->file = temp_filename;
	}
	unlink(temp_path);
	return (temp_fd);
}
