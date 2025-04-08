/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:24:16 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/08 18:43:36 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	*ft_lstfree_t(t_list *tok_h)
{
	t_token	*tok;
	t_list	*current;

	if (!tok_h)
		return (NULL);
	ft_lstiter(tok_h, free);
	current = tok_h;
	while (current)
	{
		tok = (t_token *) current->content;
		free(tok->value);
		current = tok_h->next;
		free(tok_h);
		tok_h = NULL;
	}
	return (NULL);
}

void	*ft_tok_err(int *p_status, int status)
{
	*p_status = status;
	return (NULL);
}

void	ft_print_list(t_list *tok_h)
{
	t_list	*tok_curr;
	t_token	*tok;

	tok_curr = tok_h;
	while (tok_curr)
	{
		tok = (t_token *) tok_curr->content;
		ft_printerr("%s: %d\n", tok->value, tok->type);
		tok_curr = tok_curr->next;
	}
}
