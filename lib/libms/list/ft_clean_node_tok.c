/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_node_tok.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:17:55 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 22:03:07 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libms.h"

/**
 * @brief Frees a token node and its content
 * 
 * @param node The token node to free
 * @return void* Always NULL
 */
void	*ft_clean_node_tok(t_list *node)
{
	if (!node || node->type != TYPE_TOKEN)
		return (NULL);
	if ((node->type == TYPE_TOKEN) && node->content.token)
	{
		if (node->content.token->value)
			free(node->content.token->value);
		free(node->content.token);
		node->content.token = NULL;
	}
	free(node);
	return (NULL);
}
