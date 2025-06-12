/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:25:07 by alearroy          #+#    #+#             */
/*   Updated: 2025/06/12 18:25:22 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static void	free_strs(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static void	free_redirs(t_list *redirs)
{
	t_list	*tmp;
	t_redir	*redir;

	while (redirs)
	{
		tmp = redirs->next;
		if (redirs->type == TYPE_REDIR)
		{
			redir = redirs->content.redir;
			if (redir)
			{
				free(redir->file);
				free(redir);
			}
		}
		free(redirs);
		redirs = tmp;
	}
}

void	free_command_list(t_list *cmds)
{
	t_list		*next;
	t_command	*cmd;

	while (cmds)
	{
		next = cmds->next;
		cmd = (t_command *)cmds->content.generic;
		if (cmd)
		{
			free_strs(cmd->args);
			free_redirs(cmd->redirs);
			free(cmd);
		}
		free(cmds);
		cmds = next;
	}
}