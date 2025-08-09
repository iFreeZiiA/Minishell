/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:20:15 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 11:29:23 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_loop(const char *s, const char c, int *i)
{
	int	j;

	j = *i;
	while (s[++(*i)])
		if (s[*i] == c)
			return ;
	*i = j;
}

int	ft_find_quotes(char const *s)
{
	int	i;

	i = 0;
	if (!s || !s[i])
		return (-1);
	if (s[i] == '\'')
		ft_loop(s, '\'', &i);
	else if (s[i] == '"')
		ft_loop(s, '"', &i);
	return (i);
}
