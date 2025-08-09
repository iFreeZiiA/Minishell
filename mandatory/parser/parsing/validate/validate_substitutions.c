/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_substitutions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 10:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/08 10:00:00 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Validates input for unsupported command substitutions
 * Prevents crashes from $(command) substitutions which are not required in minishell
 * 
 * @param input The input string to validate
 * @return int 0 if valid, 1 if contains unsupported substitutions
 */
int	ft_validate_substitutions(char *input)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	if (!input)
		return (0);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (!in_single_quote && input[i] == '$' && input[i + 1] == '(')
		{
			write(2, "minishell: command substitution not supported\n", 47);
			return (1);
		}
		else if (!in_single_quote && input[i] == '$' && input[i + 1] == '{')
		{
			write(2, "minishell: parameter expansion not supported\n", 46);
			return (1);
		}
		i++;
	}
	return (0);
}
