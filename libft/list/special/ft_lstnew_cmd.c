/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:23:58 by alearroy          #+#    #+#             */
/*   Updated: 2025/04/16 18:47:08 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_list	*ft_lstnew_cmd(t_command *cmd)
{
	t_list	*new;

	if (!cmd)
		return (NULL);
	new = (t_list *) malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content.cmd = cmd;
	new->type = TYPE_COMMAND;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}