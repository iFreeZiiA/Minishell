/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:06:43 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_copy_token_to_list(t_list **clean_list, t_token *original_token)
{
	t_token		*token_copy;
	t_content	content;

	token_copy = malloc(sizeof(t_token));
	if (!token_copy)
		return (0);
	token_copy->type = original_token->type;
	token_copy->value = ft_strdup(original_token->value);
	if (!token_copy->value)
	{
		free(token_copy);
		return (0);
	}
	content.token = token_copy;
	if (!ft_lstadd_back(clean_list, content, TYPE_TOKEN))
	{
		free(token_copy->value);
		free(token_copy);
		return (0);
	}
	return (1);
}

t_list	*ft_create_clean_token_list(t_list *tokens)
{
	t_list	*clean_list;
	t_list	*curr;

	clean_list = NULL;
	curr = tokens;
	while (curr && curr->content.token->type != TOKEN_PIPE)
	{
		if (!ft_copy_token_to_list(&clean_list, curr->content.token))
		{
			ft_free_token_list(clean_list);
			return (NULL);
		}
		curr = curr->next;
	}
	return (clean_list);
}

void	ft_free_token_list(t_list *tokens)
{
	t_list	*curr;
	t_list	*next;
	t_token	*token;

	curr = tokens;
	while (curr)
	{
		next = curr->next;
		if (curr->type == TYPE_TOKEN)
		{
			token = curr->content.token;
			if (token)
			{
				if (token->value)
					free(token->value);
				free(token);
			}
		}
		free(curr);
		curr = next;
	}
}

bool	ft_has_redirections_until_pipe(t_list *tokens)
{
	t_list	*curr;
	t_token	*token;

	curr = tokens;
	while (curr && curr->content.token->type != TOKEN_PIPE)
	{
		token = curr->content.token;
		if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT ||
			token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
			return (true);
		curr = curr->next;
	}
	return (false);
}
