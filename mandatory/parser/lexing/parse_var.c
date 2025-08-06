/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:11:31 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

char	*ft_ext_var_name(char *str, int pos)
{
	int		start;
	int		len;
	char	*var_name;

	if (!str || str[pos] != '$')
		return (NULL);
	start = pos + 1;
	len = 0;
	if (str[start] == '?')
		len = 1;
	else
	{
		while (str[start + len] && (ft_isalnum(str[start + len])
				|| str[start + len] == '_'))
			len++;
	}
	if (len == 0)
		return (NULL);
	var_name = ft_substr(str, start, len);
	return (var_name);
}

char	*ft_repl_var(char *str, int pos, int var_len, char *replacement)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(str, 0, pos);
	after = ft_strdup(str + pos + var_len);
	temp = ft_strjoin(before, replacement);
	result = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(temp);
	return (result);
}

static int	ft_process_variable(char **result, int *i, char **envp,
		t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*new_result;

	var_name = ft_ext_var_name(*result, *i);
	if (var_name)
	{
		var_value = ft_get_var_val(var_name, envp, shell);
		new_result = ft_repl_var(*result, *i, ft_strlen(var_name)
				+ 1, var_value);
		free(*result);
		free(var_name);
		free(var_value);
		*result = new_result;
		*i = 0;
		return (1);
	}
	return (0);
}

char	*ft_exp_string(char *str, char **envp, t_shell *shell)
{
	char	*result;
	int		i;

	if (!str || !envp)
		return (ft_strdup(str));
	if (ft_strlen(str) >= 2 && str[0] == '\'' && str[ft_strlen(str) - 1]
		== '\'')
		return (ft_strdup(str));
	result = ft_strdup(str);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && result[i + 1])
		{
			if (ft_process_variable(&result, &i, envp, shell))
				continue ;
		}
		i++;
	}
	return (result);
}
