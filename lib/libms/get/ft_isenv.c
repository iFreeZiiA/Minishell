/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:19:54 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 13:15:03 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libms.h"

static int	ft_is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_') || !s[1])
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

static char	*ft_loop(char **env, char *key)
{
	char	*new_key;
	int		i;
	int		end;

	if (!env || !(*env) || !key)
		return (NULL);
	new_key = ft_strjoin(key, "=");
	if (!new_key)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		end = ft_is_valid_identifier(env[i]);
		if (!ft_strncmp(env[i], new_key, end))
		{
			free(new_key);
			return (ft_strdup(&env[i][end + 1]));
		}
	}
	free(new_key);
	return (NULL);
}

static char	*ft_search_env_vars(t_shell *shell, char *key)
{
	char	*value;

	value = ft_loop(shell->env->env_vars, key);
	if (value)
		return (value);
	value = ft_loop(shell->env->local_env, key);
	if (value)
		return (value);
	return (ft_strdup(""));
}

char	*ft_getenv_value(t_shell *shell, char *str, int i, int eov)
{
	char	*value;
	char	*key;

	if (!shell || !str)
		return (NULL);
	key = ft_substr(str, i, eov);
	if (!key)
		return (NULL);
	value = ft_search_env_vars(shell, key);
	free(key);
	return (value);
}
