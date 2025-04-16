/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:08:34 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/16 18:47:20 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_list	*ft_lstnew_redir(t_redir *redir)
{
	t_list	*new;

	if (!redir)
		return (NULL);
	new = (t_list *) malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content.redir = redir;
	new->type = TYPE_REDIR;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
