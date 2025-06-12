/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:13:17 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static redir_type	ft_token_to_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (token_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	if (token_type == TOKEN_APPEND)
		return (REDIR_APPEND);
	return (REDIR_IN);
}

static t_redir	*ft_create_redirection(redir_type type, char *filename)
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

static int	ft_add_redirection_to_cmd(t_command *cmd, t_redir *redir)
{
	t_content	content;

	if (!cmd || !redir)
		return (-1);
	content.redir = redir;
	if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
		return (-1);
	return (0);
}

static int	ft_process_single_redirection(t_list *curr, t_command *cmd)
{
	t_redir		*redir;
	redir_type	type;

	if (!curr->next)
		return (-1);
	type = ft_token_to_redir_type(curr->content.token->type);
	redir = ft_create_redirection(type, curr->next->content.token->value);
	if (!redir)
		return (-1);
	if (ft_add_redirection_to_cmd(cmd, redir) < 0)
	{
		free(redir->file);
		free(redir);
		return (-1);
	}
	return (0);
}

int	ft_parse_redirections(t_shell *shell, t_list *token_h, t_command *cmd)
{
	t_list	*curr;

	if (!shell || !token_h || !cmd)
		return (-1);
	curr = token_h;
	while (curr)
	{
		if (ft_is_redirect_token(curr->content.token->type))
		{
			if (ft_process_single_redirection(curr, cmd) < 0)
				return (-1);
			curr = curr->next;
		}
		curr = curr->next;
	}
	return (0);
}
