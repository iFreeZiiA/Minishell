/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:14:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	*ft_build_result_with_marker(char *var_name, char *marker_pos)
{
	char	*result;
	char	*temp;

	result = ft_strjoin(var_name, "=");
	temp = result;
	result = ft_strjoin(temp, marker_pos + 1);
	free(temp);
	return (result);
}

char	*ft_handle_export_arg(char *arg)
{
	char	*equals_pos;
	char	*var_name;
	char	*marker_pos;
	char	*result;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
		return (ft_strdup(arg));
	var_name = ft_substr(arg, 0, equals_pos - arg);
	marker_pos = ft_strchr(equals_pos + 1, '\x01');
	if (marker_pos)
	{
		result = ft_build_result_with_marker(var_name, marker_pos);
		free(var_name);
		return (result);
	}
	free(var_name);
	return (ft_strdup(arg));
}

int	ft_should_skip_expansion(char *cmd_name, int arg_index, char *arg)
{
	char	*equals_pos;

	if (!ft_strcmp(cmd_name, "export") && arg_index > 0)
	{
		equals_pos = ft_strchr(arg, '=');
		if (equals_pos)
		{
			if (ft_strchr(equals_pos + 1, '\x01'))
				return (1);
		}
	}
	return (0);
}

void	ft_process_special_arg(char **arg)
{
	char	*cleaned;

	cleaned = ft_handle_export_arg(*arg);
	free(*arg);
	*arg = cleaned;
}

void	ft_process_normal_arg(char **arg, char **envp, t_shell *shell)
{
	char	*expanded;
	char	*cleaned;

	expanded = ft_exp_string(*arg, envp, shell);
	if (expanded)
	{
		cleaned = ft_rm_quotes(expanded);
		free(*arg);
		free(expanded);
		*arg = cleaned;
	}
}
