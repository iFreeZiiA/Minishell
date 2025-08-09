/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adv_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 23:10:21 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_val_input_tokens(t_list *tokens)
{
	int	result;

	result = ft_val_syntax(tokens);
	if (result != 0)
		return (result);
	result = ft_val_paren_bal(tokens);
	if (result != 0)
		return (result);
	return (0);
}

static int	ft_val_cmd_struct(t_list *tokens)
{
	int	result;

	result = ft_val_comp_cmds(tokens);
	if (result != 0)
		return (result);
	result = ft_val_heredoc_syn(tokens);
	if (result != 0)
		return (result);
	return (0);
}

int	ft_val_comp_syntax(char *input, t_list *tokens)
{
	int	result;

	if (!input)
		return (0);
	result = ft_validate_quotes_in_input(input);
	if (result != 0)
		return (result);
	if (!tokens)
		return (0);
	result = ft_val_input_tokens(tokens);
	if (result != 0)
		return (result);
	result = ft_val_cmd_struct(tokens);
	if (result != 0)
		return (result);
	return (0);
}

int	ft_is_empty_ws(char *input)
{
	int	i;

	if (!input)
		return (1);
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_check_paren(int *paren_count)
{
	(*paren_count)--;
	if (*paren_count < 0)
	{
		ft_printerr("bash: syntax error near unexpected token `)'\n");
		return (2);
	}
	return (0);
}
