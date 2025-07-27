/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:36:36 by alearroy          #+#    #+#             */
/*   Updated: 2025/06/24 17:39:16 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "\n", 1);
}

static int	heredoc_loop(int *pipe_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_sig == SIGINT)
		{
			free(line);
			return (-1);
		}
		if (!line)
			break;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	free(line);
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	void	(*prev_sigint)(int);
	int		ret;

	pipe(pipe_fd);
	prev_sigint = signal(SIGINT, handle_heredoc_sigint);
	g_sig = 0;
	ret = heredoc_loop(pipe_fd, delimiter);
	close(pipe_fd[1]);
	signal(SIGINT, prev_sigint);
	if (ret == -1)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
