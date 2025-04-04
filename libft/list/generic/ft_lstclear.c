/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:38:01 by jjorda            #+#    #+#             */
/*   Updated: 2024/11/10 10:43:11 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

/**
 * Clears the entire linked list, deleting each element and freeing its memory.
 * 
 * @param lst The pointer to the first element of the linked list.
 * @param del The function to delete the content of each element.
 * 
 * Note: The function uses free() to release the memory allocated for each node.
 */
void	ft_lstclear(t_list **lst, void (*del)(mlx_t *, void *))
{
	t_list	*current;

	if (!*lst)
		return ;
	current = *lst;
	while (current)
	{
		*lst = current->next;
		del(current->mlx, (void *) current->content);
		free(current);
		current = *lst;
	}
}
