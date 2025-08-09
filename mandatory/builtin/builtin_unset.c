/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:35:51 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 10:59:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static int	ft_allocate_and_check(char **env, char ***new)
{
	int	count;
	int	i;

	i = 0;
	while (env[i])
		i++;
	count = i;
	*new = malloc(sizeof(char *) * count);
	if (*new != NULL)
		return (count);
	return (-1);
}

static int	ft_copy_valid_env(char **env, char **new, int skip)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (env[i])
	{
		if (i != skip)
		{
			new[count] = ft_strdup(env[i]);
			if (!new[count])
			{
				while (--count >= 0)
					free(new[count]);
				free(new);
				return (-1);
			}
			count++;
		}
		i++;
	}
	new[count] = NULL;
	return (count);
}

char	**alloc_env(char **env, int skip)
{
	char	**new;
	int		result;

	if (ft_allocate_and_check(env, &new) == -1)
		return (NULL);
	result = ft_copy_valid_env(env, new, skip);
	if (result == -1)
		return (NULL);
	return (new);
}

static void	ft_free_env_unset(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

int	builtin_unset(char **args, char ***env)
{
	int		idx;
	char	**new;

	if (!args[1])
		return (0);
	idx = find_env_var(*env, args[1]);
	if (idx < 0)
		return (0);
	new = alloc_env(*env, idx);
	if (!new)
		return (1);
	ft_free_env_unset(*env);
	*env = new;
	return (0);
}
