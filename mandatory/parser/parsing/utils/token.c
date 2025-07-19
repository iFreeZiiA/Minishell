/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 15:40:06 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_is_word_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TOKEN_WORD)
		return (1);
	return (0);
}

int	ft_is_operator_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TOKEN_PIPE)
		return (1);
	if (token->type == TOKEN_AND)
		return (1);
	if (token->type == TOKEN_OR)
		return (1);
	return (0);
}

int	ft_is_redirect_token_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (1);
	if (type == TOKEN_REDIR_OUT)
		return (1);
	if (type == TOKEN_HEREDOC)
		return (1);
	if (type == TOKEN_APPEND)
		return (1);
	return (0);
}

int	ft_count_tokens_by_type(t_list *tokens, t_token_type type)
{
	t_list	*current;
	int		count;

	current = tokens;
	count = 0;
	while (current)
	{
		if (current->content.token->type == type)
			count++;
		current = current->next;
	}
	return (count);
}

t_list	*ft_find_first_token_type(t_list *tokens, t_token_type type)
{
	t_list	*current;

	current = tokens;
	while (current)
	{
		if (current->content.token->type == type)
			return (current);
		current = current->next;
	}
	return (NULL);
}
