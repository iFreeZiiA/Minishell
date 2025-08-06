/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 21:17:17 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_validate_input_tokens(t_list *tokens)
{
	int	result;

	result = ft_validate_syntax(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_parentheses_balance(tokens);
	if (result != 0)
		return (result);
	return (0);
}

static int	ft_validate_command_structure(t_list *tokens)
{
	int	result;

	result = ft_validate_compound_commands(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_heredoc_syntax(tokens);
	if (result != 0)
		return (result);
	return (0);
}

int	ft_validate_complete_syntax(char *input, t_list *tokens)
{
	int	result;

	if (!input)
		return (0);
	result = ft_validate_quotes_in_input(input);
	if (result != 0)
		return (result);
	if (!tokens)
		return (0);
	result = ft_validate_input_tokens(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_command_structure(tokens);
	if (result != 0)
		return (result);
	return (0);
}

int	ft_is_empty_or_whitespace(char *input)
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

int	ft_check_paren_close(int *paren_count)
{
	(*paren_count)--;
	if (*paren_count < 0)
	{
		ft_printerr("bash: syntax error near unexpected token `)'\n");
		return (2);
	}
	return (0);
}
