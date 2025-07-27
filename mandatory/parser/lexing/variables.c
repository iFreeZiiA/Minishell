/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/27 18:43:31 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

int	ft_env_var_match(char *env_line, char *var_name)
{
	int	i;

	i = 0;
	while (var_name[i] && env_line[i])
	{
		if (var_name[i] != env_line[i])
			return (0);
		i++;
	}
	if (var_name[i] == '\0' && env_line[i] == '=')
		return (1);
	return (0);
}

static char	*ft_get_exit_code(t_shell *shell)
{
	if (shell && shell->env)
		return (ft_itoa(shell->env->last_exit_code));
	else
		return (ft_strdup("0"));
}

char	*ft_expand_var(char *str, char **envp, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	int		i;

	if (!str || str[0] != '$' || !envp)
		return (ft_strdup(str));
	var_name = str + 1;
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_get_exit_code(shell));
	i = 0;
	while (envp[i])
	{
		if (ft_env_var_match(envp[i], var_name))
		{
			var_value = envp[i] + ft_strlen(var_name) + 1;
			result = ft_strdup(var_value);
			return (result);
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*ft_get_var_value(char *var_name, char **envp, t_shell *shell)
{
	char	*var_value;
	int		i;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_get_exit_code(shell));
	i = 0;
	while (envp[i])
	{
		if (ft_env_var_match(envp[i], var_name))
		{
			var_value = envp[i] + ft_strlen(var_name) + 1;
			return (ft_strdup(var_value));
		}
		i++;
	}
	return (ft_strdup(""));
}
