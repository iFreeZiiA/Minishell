/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:05:13 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 13:23:23 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_lstfree(t_list *list)
{
	t_list	*current;

	if (!list)
		return (NULL);
	ft_lstiter(list, free);
	current = list;
	while (current)
	{
		current = list->next;
		free(list);
		list = NULL;
	}
	return (NULL);
}
