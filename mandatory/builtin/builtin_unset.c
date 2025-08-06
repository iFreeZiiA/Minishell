/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:35:51 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/02 12:40:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	**alloc_env(char **env, int skip)
{
	int		i;
	int		count;
	char	**new;

	i = 0;
	count = 0;
	while (env[i])
		i++;
	new = malloc(sizeof(char *) * i);
	if (!new)
		return (NULL);
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
				return (NULL);
			}
			count++;
		}
		i++;
	}
	new[count] = NULL;
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
	idx = get_env_index(*env, args[1]);
	if (idx < 0)
		return (0);
	new = alloc_env(*env, idx);
	if (!new)
		return (1);
	ft_free_env_unset(*env);
	*env = new;
	return (0);
}
