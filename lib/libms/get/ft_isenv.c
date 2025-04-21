/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:19:54 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/21 17:56:02 by jjorda           ###   ########.fr       */
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

static char	*ft_loop(char **env, const char *key)
{
	int		i;
	int		end;

	if (!env || !(*env) || !key)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		end = ft_is_valid_identifier(env[i]);
		if (!ft_strncmp(env[i], key, end))
			return (ft_strdup(&env[i][end + 1]));
	}
	return (NULL);
}

char	*ft_getenv_value(t_shell *shell, const char *key)
{
	char	**env;
	char	*value;
	char	*new_key;

	if (!shell || !key)
		return (NULL);
	new_key = ft_strjoin(key, "=");
	if (!new_key)
		return (NULL);
	env = shell->env->env_vars;
	value = ft_loop(env, new_key);
	if (value)
	{
		free(new_key);
		return (value);
	}
	env = shell->env->local_env;
	value = ft_loop(env, new_key);
	free(new_key);
	if (value)
		return (value);
	return (ft_strdup(""));
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