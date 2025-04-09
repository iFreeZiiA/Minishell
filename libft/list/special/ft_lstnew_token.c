/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_tok.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:16:34 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/09 19:06:28 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_list	*ft_lstnew_tok(t_token *token)
{
	t_list	*new;

	if (!token)
		return (NULL);
	new = (t_list *) malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content.token = token;
	new->type = TYPE_TOKEN;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
