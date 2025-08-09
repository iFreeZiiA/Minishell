/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstleak.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:05:42 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 13:22:44 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_lstleak(t_list **lst, void *(*f)(t_list *))
{
	t_list	*prev;
	t_list	*curr;

	if (!(*lst))
		return (NULL);
	curr = *lst;
	while (curr)
	{
		prev = curr->prev;
		f(curr);
		curr = prev;
	}
	*lst = NULL;
	return (NULL);
}
