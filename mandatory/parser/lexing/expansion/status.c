/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:04:01 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/29 16:36:42 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	ft_loop_status(char *str, char *new_str, char *status)
{
	bool	stat;
	int		i;
	int		j;
	int		k;

	i = -1;
	j = 0;
	stat = false;
	while (str[++i])
	{
		if (str[i] == DOLL && str[i + 1] && str[i + 1] == Q_MRK && !stat)
		{
			k = -1;
			while (status[++k])
				new_str[j++] = status[k];
			i += 2;
			stat = true;
		}
		new_str[j] = str[i];
		j++;
	}
	new_str[j] = '\0';
}

bool	ft_expand_status(t_shell *shell, t_list *tok_c, char *str)
{
	char	*new_str;
	char	*status;
	int		size;

	if (!shell || !str)
		return (false);
	status = ft_itoa(shell->env->last_exit_code);
	size = ft_strlen(status);
	new_str = (char *) malloc(((ft_strlen(str) - 2) + size + 1) * sizeof(char));
	if (!new_str)
		return (false);
	ft_loop_status(str, new_str, status);
	free(str);
	tok_c->content.token->value = new_str;
	return (true);
}