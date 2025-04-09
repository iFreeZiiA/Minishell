/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstrem_first.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:40:41 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/09 17:36:57 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

bool	ft_lstrem_front(t_list **actual, void (*f)(void *))
{
	t_list	*first;
	t_list	*second;

	if (!(*actual) || !f)
		return (false);
	first = ft_lstfirst(*actual);
	second = NULL;
	if (first->next)
		second = first->next;
	if (*actual == first)
		*actual = second;
	f((void *) first->content);
	free(first);
	first = NULL;
	if (second)
		second->prev = NULL;
	return (true);
}
