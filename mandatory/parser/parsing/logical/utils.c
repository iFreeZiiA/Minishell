/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precedence_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:45:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:53:02 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

t_token	*ft_create_token_copy_precedence(t_list *curr)
{
	t_token	*token_copy;

	token_copy = malloc(sizeof(t_token));
	if (!token_copy)
		return (NULL);
	token_copy->type = curr->content.token->type;
	token_copy->value = ft_strdup(curr->content.token->value);
	if (!token_copy->value)
	{
		free(token_copy);
		return (NULL);
	}
	return (token_copy);
}

int	ft_add_token_to_sublist_precedence(t_list **result, t_token *token_copy)
{
	t_list	*new_node;
	t_list	*last;

	new_node = ft_lstnew_tok(token_copy);
	if (!new_node)
		return (0);
	if (!*result)
		*result = new_node;
	else
	{
		last = *result;
		while (last->next)
			last = last->next;
		last->next = new_node;
		new_node->prev = last;
	}
	return (1);
}

static int	ft_process_token_copy(t_list **result, t_list *curr)
{
	t_token	*token_copy;

	token_copy = ft_create_token_copy_precedence(curr);
	if (!token_copy)
	{
		ft_lstfree_t(*result);
		return (0);
	}
	if (!ft_add_token_to_sublist_precedence(result, token_copy))
	{
		free(token_copy->value);
		free(token_copy);
		ft_lstfree_t(*result);
		return (0);
	}
	return (1);
}

t_list	*ft_create_token_sublist_precedence(t_list *start, t_list *end)
{
	t_list	*result;
	t_list	*curr;

	if (!start || !end)
		return (NULL);
	result = NULL;
	curr = start;
	while (curr && curr != end->next)
	{
		if (!ft_process_token_copy(&result, curr))
			return (NULL);
		curr = curr->next;
	}
	return (result);
}

void	ft_free_token_sublist_precedence(t_list *sublist)
{
	t_list	*curr;
	t_list	*next;

	curr = sublist;
	while (curr)
	{
		next = curr->next;
		if (curr->content.token)
		{
			free(curr->content.token->value);
			free(curr->content.token);
		}
		free(curr);
		curr = next;
	}
}
