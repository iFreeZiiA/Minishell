/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:58:25 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 16:41:37 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	print_export(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}

static void	ft_process_export_arg(char *arg, char ***env)
{
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		*equals_pos = '\0';
		update_env_var(env, arg, equals_pos + 1);
		*equals_pos = '=';
	}
	else
	{
		update_env_var(env, arg, "");
	}
}

int	builtin_export(char **args, char ***env)
{
	int	i;

	if (!args[1])
		return (print_export(*env));
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else
		{
			ft_process_export_arg(args[i], env);
		}
		i++;
	}
	return (0);
}
