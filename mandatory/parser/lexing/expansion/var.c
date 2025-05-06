/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:09:58 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 17:54:30 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	ft_loop(char *str, char *new_str, char *var, char *to_rep)
{
	int	i;
	int	j;
	int	k;

	(void) var;
	i = -1;
	j = 0;
	// ft_printerr("ft_loop: %s\n", str);
	while (str[++i])
	{
		if (str[i] == DOLL && ft_strncmp(&str[i + 1], str, ft_strlen(str)))
		{
			i++;
			k = -1;
			while (to_rep[++k])
				new_str[j++] = to_rep[k];
			i += ft_eov(&str[i]);
		}
		new_str[j] = str[i];
		j++;
	}
	// ft_printerr("%c, %d/%d\n", str[i], i, j);
	new_str[j] = '\0';
	// ft_printerr("ft_loop: %s, %d\n", new_str, ft_strlen(str));
}

bool	ft_expand_var(t_shell *shell, t_list *tok_c, char *var, int i)
{
	char	*str;
	char	*new_str;
	char	*to_rep;
	int		size;

	if (!shell || !tok_c || !var)
		return (false);
	(void)var;
	str = tok_c->content.token->value;
	size = ft_eov(&str[i]);
	to_rep = ft_getenv_value(shell, str, i, size);
	new_str = (char *) ft_calloc(sizeof(char), ft_strlen(str) - size
		+ ft_strlen(to_rep) + 1);
	if (!new_str)
		return (false);
	ft_loop(str, new_str, str, to_rep);
	free(str);
	tok_c->content.token->value = new_str;
	return (true);
}
