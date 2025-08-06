/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:24:51 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:55:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Validates operator placement in sequence
 * 
 * @param tokens Token list to validate
 * @return int 1 if valid, 0 if invalid
 */
int	ft_val_ops(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*next_token;

	current = tokens;
	while (current && current->next)
	{
		token = (t_token *)current->content.token;
		next_token = (t_token *)current->next->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		{
			if (next_token->type == TOKEN_AND || next_token->type == TOKEN_OR
				|| next_token->type == TOKEN_PIPE)
				return (0);
		}
		if (token->type == TOKEN_PIPE)
		{
			if (next_token->type == TOKEN_AND || next_token->type == TOKEN_OR
				|| next_token->type == TOKEN_PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	ft_validate_first_token(t_list *tokens)
{
	t_token	*token;

	if (!tokens)
		return (1);
	token = (t_token *)tokens->content.token;
	if (ft_is_redir_tok(token))
		return (0);
	return (1);
}

static int	ft_validate_redir_sequence(t_list *current)
{
	t_token	*token;
	t_token	*next_token;

	token = (t_token *)current->content.token;
	if (ft_is_redir_tok(token))
	{
		if (!current->next)
			return (0);
		next_token = (t_token *)current->next->content.token;
		if (next_token->type != TOKEN_WORD)
			return (0);
	}
	return (1);
}

int	ft_val_redir(t_list *tokens)
{
	t_list	*current;

	if (!ft_validate_first_token(tokens))
		return (0);
	current = tokens;
	while (current)
	{
		if (!ft_validate_redir_sequence(current))
			return (0);
		current = current->next;
	}
	return (1);
}
