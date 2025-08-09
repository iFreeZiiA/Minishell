/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 10:31:28 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	ft_exit_check(char *input)
{
	if (!input || !ft_strcmp(input, "exit"))
	{
		if (input)
			free(input);
		write(1, "exit\n", 5);
		return (-1);
	}
	return (0);
}

char	*ft_read_input(int is_interactive)
{
	char	*input;
	size_t	len;

	if (is_interactive)
		input = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
	else
	{
		len = 0;
		input = NULL;
		if (getline(&input, &len, stdin) == -1)
		{
			free(input);
			return (NULL);
		}
		len = ft_strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
	}
	return (input);
}

static int	ft_check_substitutions(char *input, t_shell *shell,
							int is_interactive)
{
	if (ft_validate_substitutions(input) != 0)
	{
		shell->last_exit_code = EXIT_SYNTAX;
		if (!is_interactive)
			return (-1);
		return (1);
	}
	return (0);
}

int	ft_validate_input(char *input, t_shell *shell, int is_interactive)
{
	int	syntax_result;

	if (ft_strlen(input) > 100000)
	{
		write(2, "minishell: command too long\n", 29);
		return (1);
	}
	if (ft_is_empty_ws(input))
		return (1);
	syntax_result = ft_validate_quotes_in_input(input);
	if (syntax_result != 0)
	{
		shell->last_exit_code = EXIT_SYNTAX;
		if (!is_interactive)
			return (-1);
		return (1);
	}
	return (ft_check_substitutions(input, shell, is_interactive));
}
