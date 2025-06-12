/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:12:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_count_command_args(t_list *start, t_list *end)
{
	t_list	*curr;
	int		count;

	curr = start;
	count = 0;
	while (curr && curr != end)
	{
		if (ft_is_word_token(curr->content.token))
			count++;
		else if (ft_is_redirect_token(curr->content.token->type))
			curr = curr->next;
		if (curr)
			curr = curr->next;
	}
	return (count);
}

static char	**ft_allocate_args_array(int arg_count)
{
	char	**args;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[arg_count] = NULL;
	return (args);
}

static char	**ft_fill_args_array(t_list *start, t_list *end, int arg_count)
{
	char	**args;
	t_list	*curr;
	int		i;

	args = ft_allocate_args_array(arg_count);
	if (!args)
		return (NULL);
	curr = start;
	i = 0;
	while (curr && curr != end && i < arg_count)
	{
		if (ft_is_word_token(curr->content.token))
		{
			args[i] = ft_strdup(curr->content.token->value);
			if (!args[i])
				return (ft_leak(args, i));
			i++;
		}
		else if (ft_is_redirect_token(curr->content.token->type))
			curr = curr->next;
		if (curr)
			curr = curr->next;
	}
	return (args);
}

t_ast_node	*ft_parse_command(t_list *start, t_list *end)
{
	char	**args;
	int		arg_count;

	if (!start || start == end)
		return (NULL);
	arg_count = ft_count_command_args(start, end);
	if (arg_count == 0)
		return (NULL);
	args = ft_fill_args_array(start, end, arg_count);
	if (!args)
		return (NULL);
	return (ft_create_command_node(args, NULL));
}
