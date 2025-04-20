/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstreplace_n.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:25:54 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/20 14:50:54 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool ft_lstreplace_n(t_list **del, t_list *rep, void *(*f)(t_list *), int n)
{
    t_list  *curr;
    t_list  *next;
    int     i;

    if (!del || !(*del) || !rep || n < 0 || !f)
        return (false);
    rep->prev = (*del)->prev;
    if ((*del)->prev)
        (*del)->prev->next = rep;
    curr = *del;
    i = 0;
    while (i < n && curr)
    {
        next = curr->next;
        f(curr);
        curr = next;
        i++;
    }
    rep->next = curr;
    if (curr)
        curr->prev = rep;
    *del = rep;
    return (true);
}
