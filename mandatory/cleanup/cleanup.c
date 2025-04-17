/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:45:35 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/17 15:18:04 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static inline void	ft_freelst(t_list *lst, int type)
{
	t_list	*curr;

	if (!lst)
		return ;
	curr = lst;
	while (curr)
	{
		lst = curr->next;
		if (type == 1)
		{
			free(curr->content.token);
			curr->content.token = NULL;
		}
		else if (type == 2)
		{
			free(curr->content.redir);
			curr->content.redir = NULL;
		}
		else
		{
			free(curr->content.generic);
			curr->content.generic = NULL;
		}
		curr = lst;
	}
}

static inline void	ft_freeast(t_ast_node *ast)
{
	if (!ast)
		return ;
	if (ast->type == NODE_REDIR)
		ft_freelst(ast->data, 2);
	if (ast->type == NODE_COMMAND)
		free(ast->data);
}

static inline void	ft_freetoken(t_list *tok_h)
{
	t_list	*tok_c;

	if (!tok_h)
		return ;
	tok_c = tok_h;
	while (tok_c)
	{
		tok_h = tok_c->next;
		free(tok_c->content.token);
		tok_c->content.token = NULL;
		tok_c = tok_h;
	}
}

void	ft_cleanup(t_shell *shell, int status)
{
	if (!shell)
		return ;
	if (shell->env)
		free(shell->env);
	ft_freetoken(shell->token);
	ft_freeast(shell->ast);
}
