/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstreplace_n.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:25:54 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 17:42:07 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool	ft_lstreplace_n(t_list **del, t_list *rep, void *(*f) (t_list *), int n)
{
	int		i;

	if (!del || !(*del) || !rep)
		return (false);
	i = -1;
	rep->prev = NULL;
	if ((*del)->prev)
	{
		rep->prev = (*del)->prev;
		rep->prev->next = rep;
	}
	while (++i < n && *del)
	{
		rep->next = (*del)->next;
		f(*del);
		*del = rep->next;
	}
	return (true);
}