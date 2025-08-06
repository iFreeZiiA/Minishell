/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:55:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_val_pipes(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command;

	current = tokens;
	has_command = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PIPE)
		{
			if (!has_command)
				return (0);
			has_command = 0;
		}
		else if (token->type == TOKEN_WORD)
			has_command = 1;
		current = current->next;
	}
	if (!has_command && tokens)
		return (0);
	return (1);
}

static int	ft_val_logical_ops(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command;

	current = tokens;
	has_command = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		{
			if (!has_command)
				return (0);
			has_command = 0;
		}
		else if (token->type == TOKEN_WORD)
			has_command = 1;
		current = current->next;
	}
	if (!has_command && tokens)
		return (0);
	return (1);
}

static int	ft_val_paren(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		balance;

	current = tokens;
	balance = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			balance++;
		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			balance--;
			if (balance < 0)
				return (0);
		}
		current = current->next;
	}
	return (balance == 0);
}

int	ft_val_syntax(t_list *tokens)
{
	if (!tokens)
		return (0);
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
