/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:23:38 by jjorda            #+#    #+#             */
/*   Updated: 2025/02/19 20:17:21 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

/**
 * Creates a new linked list node with the given line.
 * 
 * @param line The line to store in the new node.
 * @return A pointer to the new node, or NULL if malloc fails.
 * 
 * Note: Memory for the new node is allocated using malloc(). 
 *       If malloc fails, the function returns NULL.
 */
t_map	*ft_lstnew_map(char *line)
{
	t_map	*new_node;

	new_node = (t_map *) malloc(sizeof(t_map));
	if (!new_node)
		return (NULL);
	new_node->line = line;
	new_node->size_line = ft_strlen(line);
	new_node->next = NULL;
	return (new_node);
}
