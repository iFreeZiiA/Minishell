/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:19:54 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/10 17:11:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (-1);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (-1);
		i++;
	}
	return (i);
}

char	*ft_isenv(char **env, const char *key)
{
	int		i;
	int		end;

	if (!env || !(*env) || !key)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		end = ft_is_valid_identifier(env[i]);
		if (!ft_strncmp(env[i], key, end - 1))
			return (ft_substr(env[i], 0, end));
	}
	return (NULL);
}
