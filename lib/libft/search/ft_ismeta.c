/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ismeta.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:26:46 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 13:21:42 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool	ft_ismeta(char *meta, char c)
{
	if (!meta)
		return (false);
	while (*meta)
	{
		if (c == *meta)
			return (true);
		meta++;
	}
	return (false);
}
