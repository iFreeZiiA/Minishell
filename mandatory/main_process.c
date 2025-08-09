/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:22:55 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	ft_handle_parse_error(t_shell *shell, int parse_result, int is_interactive)
{
	if (parse_result == EXIT_SYNTAX)
		shell->last_exit_code = EXIT_SYNTAX;
	else
		shell->last_exit_code = EXIT_FAILURE;
	if (shell->ast)
	{
		ft_free_ast(shell->ast);
		shell->ast = NULL;
	}
	if (!is_interactive)
		return (-1);
	return (0);
}

int	ft_process_command(char *input, t_shell *shell, int is_interactive)
{
	int		parse_result;
	int		exec_result;
	char	**env_backup;

	parse_result = ft_parse_input(input, shell);
	if (parse_result == 0 && shell->ast)
	{
		env_backup = shell->env->env_vars;
		exec_result = executor_from_ast(shell->ast, shell->env);
		shell->last_exit_code = exec_result;
		if (shell->env->env_vars != env_backup)
		{
			if (!shell->env->env_vars)
			{
				shell->env->env_vars = env_backup;
				shell->last_exit_code = EXIT_FAILURE;
			}
		}
		ft_free_ast(shell->ast);
		shell->ast = NULL;
	}
	else
		return (ft_handle_parse_error(shell, parse_result, is_interactive));
	return (0);
}

static int	ft_handle_validation_error(char *input, int validation_result)
{
	if (validation_result == -1)
	{
		free(input);
		return (-1);
	}
	if (validation_result == 1)
	{
		free(input);
		return (0);
	}
	return (1);
}

int	ft_handle_input(t_shell *shell, int is_interactive)
{
	char	*input;
	int		validation_result;
	int		handle_result;

	input = ft_read_input(is_interactive);
	if (ft_exit_check(input) == -1)
		return (-1);
	shell->current_line = input;
	validation_result = ft_validate_input(input, shell, is_interactive);
	handle_result = ft_handle_validation_error(input, validation_result);
	if (handle_result != 1)
		return (handle_result);
	add_history(input);
	if (ft_process_command(input, shell, is_interactive) == -1)
	{
		free(input);
		return (-1);
	}
	free(input);
	return (0);
}
