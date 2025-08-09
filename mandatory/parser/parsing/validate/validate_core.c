/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/08 20:34:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_process_pipe_token(t_token *token, int *has_command,
		int *consecutive_pipes);
int	ft_process_logical_token(t_token *token, int *has_command,
		int *consecutive_ops);
int	ft_check_paren_open(t_list *current, int *balance);
int	ft_check_paren_close(t_token *prev_token, int *balance);
int	ft_validate_token_size(t_list *tokens);
int	ft_check_single_token(t_list *tokens);
int	ft_process_pipe_validation(t_list *current, int *has_command,
		int *consecutive_pipes);
int	ft_process_logical_validation(t_list *current, int *has_command,
		int *consecutive_ops);

int	ft_val_pipes(t_list *tokens)
{
	int	has_command;
	int	consecutive_pipes;

	has_command = 0;
	consecutive_pipes = 0;
	if (!ft_process_pipe_validation(tokens, &has_command, &consecutive_pipes))
		return (0);
	return (has_command || !tokens);
}

static int	ft_val_logical_ops(t_list *tokens)
{
	int	has_command;
	int	consecutive_ops;

	has_command = 0;
	consecutive_ops = 0;
	if (!ft_process_logical_validation(tokens, &has_command, &consecutive_ops))
		return (0);
	return (has_command || !tokens);
}

static int	ft_val_paren(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*prev_token;
	int		balance;

	current = tokens;
	balance = 0;
	prev_token = NULL;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
		{
			if (!ft_check_paren_open(current, &balance))
				return (0);
		}
		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			if (!ft_check_paren_close(prev_token, &balance))
				return (0);
		}
		prev_token = token;
		current = current->next;
	}
	return (balance == 0);
}

int	ft_val_syntax(t_list *tokens)
{
	if (!tokens)
		return (2);
	if (!ft_validate_token_size(tokens))
		return (2);
	if (!ft_check_single_token(tokens))
		return (2);
	if (!ft_val_ops(tokens))
		return (2);
	if (!ft_val_pipes(tokens))
		return (2);
	if (!ft_val_redir(tokens))
		return (2);
	if (!ft_val_logical_ops(tokens))
		return (2);
	if (!ft_val_paren(tokens))
		return (2);
	return (0);
}
