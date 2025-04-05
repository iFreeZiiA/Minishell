/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:08:07 by jjorda            #+#    #+#             */
/*   Updated: 2024/11/11 21:56:40 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

/**
 * Adds a new element at the end of the linked list.
 * 
 * @param lst The pointer to the first element of the linked list.
 * @param new The new element to add at the end of the list.
 */
bool	ft_lstadd_back(t_list **lst, void *data)
{
	t_list	*new;
	t_list	*last;

	new = ft_lstnew(data);
	if (!new)
		return (false);
	if (!*lst)
		*lst = new;
	else
	{
		last = ft_lstlast(*lst);
		last->next = new;
	}
	return (true);
}
