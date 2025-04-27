/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:19:54 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/27 17:14:59 by jjorda           ###   ########.fr       */
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
		if (!ft_strncmp(env[i], key, end))
		{
			free(new_key);
			return (ft_strdup(&env[i][end + 1]));
		}
	}
	free(new_key);
	return (ft_strdup(""));
}

char	*ft_getenv_value(t_shell *shell, char *str, int i, int eov)
{
	char	**env;
	char	*value;
	char	*key;

	if (!shell || !str)
		return (NULL);
	key = ft_substr(str, i, ft_eov(&str[i]));
	if (!key)
		return (NULL);
	env = shell->env->env_vars;
	value = ft_loop(env, key);
	if (value)
	{
		free(key);
		return (value);
	}
	env = shell->env->local_env;
	value = ft_loop(env, key);
	free(key);
	return (value);
}

// int	main(int argc, char **argv, char **env)
// {
// 	t_shell	shell;
// 	char	**local_env;
// 	char	*str;

// 	local_env = malloc(sizeof(char *) * 2);
// 	if (!local_env)
// 		return (1);
// 	local_env[0] = ft_strdup("VAR=ok");
// 	if (!local_env[0])
// 	{
// 		free(local_env);
// 		return (1);
// 	}
// 	local_env[1] = NULL;
// 	shell.env = malloc(sizeof(t_env));
// 	shell.env->env_vars = env;
// 	shell.env->local_env = local_env;
// 	str = ft_getenv_value(&shell, argv[1]);
// 	free(local_env[0]);
// 	free(local_env);
// 	free(shell.env);
// 	if (!str)
// 		return (1);
// 	ft_printerr("FINAL_VAR='%s'\n", str);
// 	free(str);
// 	return (0);
// }