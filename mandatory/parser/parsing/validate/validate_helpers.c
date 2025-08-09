/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:38:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_process_pipe_token(t_token *token, int *has_command,
		int *consecutive_pipes)
{
	(void)token;
	(*consecutive_pipes)++;
	if (*consecutive_pipes > 1)
		return (0);
	if (!(*has_command))
		return (0);
	*has_command = 0;
	return (1);
}

int	ft_process_logical_token(t_token *token, int *has_command,
		int *consecutive_ops)
{
	(void)token;
	(*consecutive_ops)++;
	if (*consecutive_ops > 1)
		return (0);
	if (!(*has_command))
		return (0);
	*has_command = 0;
	return (1);
}

int	ft_check_paren_open(t_list *current, int *balance)
{
	t_token	*token;
	t_token	*next;

	token = (t_token *)current->content.token;
	(void)token;
	(*balance)++;
	if (current->next)
	{
		next = (t_token *)current->next->content.token;
		if (next->type == TOKEN_PIPE || next->type == TOKEN_AND
			|| next->type == TOKEN_OR || ft_is_redir_tok(next))
			return (0);
	}
	return (1);
}

int	ft_check_paren_close(t_token *prev_token, int *balance)
{
	(*balance)--;
	if (*balance < 0)
		return (0);
	if (prev_token && (prev_token->type == TOKEN_PIPE
			|| prev_token->type == TOKEN_AND || prev_token->type == TOKEN_OR
			|| ft_is_redir_tok(prev_token)))
		return (0);
	return (1);
}
