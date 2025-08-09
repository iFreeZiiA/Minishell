/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:35:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 14:16:57 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_validate_token_size(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		total_length;

	current = tokens;
	total_length = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->value)
		{
			total_length += ft_strlen(token->value);
			if (ft_strlen(token->value) > 100000)
				return (0);
		}
		if (total_length > 500000)
			return (0);
		current = current->next;
	}
	return (1);
}

int	ft_check_single_token(t_list *tokens)
{
	t_token	*token;

	if (!tokens->next)
	{
		token = (t_token *)tokens->content.token;
		if (ft_is_redir_tok(token) || token->type == TOKEN_PIPE
			|| token->type == TOKEN_AND || token->type == TOKEN_OR)
			return (0);
	}
	return (1);
}

int	ft_process_pipe_validation(t_list *current, int *has_command,
		int *consecutive_pipes)
{
	t_token	*token;

	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PIPE)
		{
			if (!ft_process_pipe_token(token, has_command, consecutive_pipes))
				return (0);
		}
		else
		{
			*consecutive_pipes = 0;
			if (token->type == TOKEN_WORD)
				*has_command = 1;
		}
		current = current->next;
	}
	return (1);
}

int	ft_process_logical_validation(t_list *current, int *has_command,
		int *consecutive_ops)
{
	t_token	*token;

	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		{
			if (!ft_process_logical_token(token, has_command, consecutive_ops))
				return (0);
		}
		else
		{
			*consecutive_ops = 0;
			if (token->type == TOKEN_WORD)
				*has_command = 1;
		}
		current = current->next;
	}
	return (1);
}
