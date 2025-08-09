/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 15:00:00 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	ft_handle_redir_in(t_redir *r)
{
	int	fd;

	fd = open(r->file, O_RDONLY);
	if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
		return (perror("minishell: redir"), close(fd), 1);
	close(fd);
	return (0);
}

static int	ft_handle_redir_out(t_redir *r)
{
	int	fd;

	fd = open(r->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
		return (perror("minishell: redir"), close(fd), 1);
	close(fd);
	return (0);
}

static int	ft_handle_redir_append(t_redir *r)
{
	int	fd;

	fd = open(r->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
		return (perror("minishell: redir"), close(fd), 1);
	close(fd);
	return (0);
}

static int	ft_handle_redir_heredoc(t_redir *r)
{
	if (r->fd < 0 || dup2(r->fd, STDIN_FILENO) < 0)
		return (perror("minishell: heredoc"), 1);
	close(r->fd);
	return (0);
}

int	apply_redirections(t_list *redirs)
{
	t_redir	*r;

	while (redirs)
	{
		if (redirs->type != TYPE_REDIR)
			return (1);
		r = redirs->content.redir;
		if (r->type == REDIR_IN && ft_handle_redir_in(r) != 0)
			return (1);
		else if (r->type == REDIR_OUT && ft_handle_redir_out(r) != 0)
			return (1);
		else if (r->type == REDIR_APPEND && ft_handle_redir_append(r) != 0)
			return (1);
		else if (r->type == REDIR_HEREDOC && ft_handle_redir_heredoc(r) != 0)
			return (1);
		else if (r->type != REDIR_IN && r->type != REDIR_OUT
			&& r->type != REDIR_APPEND && r->type != REDIR_HEREDOC)
			return (1);
		redirs = redirs->next;
	}
	return (0);
}
