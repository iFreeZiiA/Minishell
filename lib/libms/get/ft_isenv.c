/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:19:54 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 19:11:08 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libms.h"

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

static char	*ft_loop(char **env, const char *key)
{
	int		i;
	int		end;

	while (env[++i])
	{
		end = ft_is_valid_identifier(env[i]);
		if (!ft_strncmp(env[i], key, end - 1))
			return (ft_substr(env[i], 0, end));
	}
	return (NULL);
}

char	*ft_getenv_value(t_shell *shell, const char *key)
{
	char	**env;
	char	*value;

	if (!shell || !key)
		return (NULL);
	env = shell->env->env_vars;
	value = ft_loop(env, key);
	if (value)
		return (value);
	env = shell->env->local_env;
	return (ft_loop(env, key));
}
