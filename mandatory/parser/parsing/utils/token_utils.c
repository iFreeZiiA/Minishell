/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:06:43 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_count_word_tokens(t_list *tokens)
{
	t_list	*curr;
	int		count;

	curr = tokens;
	count = 0;
	while (curr)
	{
		if (curr->content.token->type == TOKEN_WORD)
			count++;
		curr = curr->next;
	}
	return (count);
}

char	**ft_extract_command_args(t_list *tokens, int count)
{
	char	**args;
	t_list	*curr;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	curr = tokens;
	i = 0;
	while (curr && i < count)
	{
		if (curr->content.token->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(curr->content.token->value);
			i++;
		}
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}

int	ft_count_word_tokens_until_pipe(t_list *tokens)
{
	t_list	*curr;
	int		count;

	curr = tokens;
	count = 0;
	while (curr && curr->content.token->type != TOKEN_PIPE)
	{
		if (curr->content.token->type == TOKEN_WORD)
			count++;
		curr = curr->next;
	}
	return (count);
}

char	**ft_extract_command_args_until_pipe(t_list *tokens, int count)
{
	char	**args;
	t_list	*curr;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	curr = tokens;
	i = 0;
	while (curr && curr->content.token->type != TOKEN_PIPE && i < count)
	{
		if (curr->content.token->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(curr->content.token->value);
			i++;
		}
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}
