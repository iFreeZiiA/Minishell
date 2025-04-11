/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:24:16 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/11 18:38:46 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	*ft_lstfree_t(t_list *tok_h)
{
	t_list	*current;

	if (!tok_h)
		return (NULL);
	current = tok_h;
	while (current)
	{
		tok_h = current->next;
		ft_clean_node_tok(current);
		current = tok_h;
	}
	return (NULL);
}

void	ft_print_list(t_list *tok_h)
{
	t_list	*tok_curr;
	t_token	*tok;

	tok_curr = tok_h;
	while (tok_curr)
	{
		tok = tok_curr->content.token;
		ft_printerr("%s: %d\n", tok->value, tok->type);
		tok_curr = tok_curr->next;
	}
}
