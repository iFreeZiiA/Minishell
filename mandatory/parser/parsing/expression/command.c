/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Counts the number of command arguments in a token range
 * 
 * This function counts word tokens (arguments) within a given range,
 * skipping over redirection tokens and their associated files.
 * 
 * @param start Starting position in token list
 * @param end Ending position in token list (exclusive)
 * @return int Number of arguments found
 */
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

/**
 * @brief Allocates memory for an array of command arguments
 * 
 * This helper function allocates memory for a null-terminated array
 * of strings to hold command arguments.
 * 
 * @param arg_count Number of arguments the array should hold
 * @return char** Pointer to allocated array, or NULL on failure
 */
static char	**ft_allocate_args_array(int arg_count)
{
	char	**args;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[arg_count] = NULL;
	return (args);
}

/**
 * @brief Fills the arguments array with command tokens
 * 
 * This function iterates through tokens in the specified range and
 * extracts word tokens to fill the arguments array, skipping redirections.
 * 
 * @param start Starting position in token list
 * @param end Ending position in token list (exclusive)
 * @param arg_count Expected number of arguments
 * @return char** Filled arguments array, or NULL on failure
 */
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

/**
 * @brief Parses a command from a token range and creates an AST node
 * 
 * This function extracts command arguments from a token range and creates
 * a command AST node. It handles the complete process of:
 * 1. Counting arguments in the range
 * 2. Allocating and filling the arguments array
 * 3. Creating the command AST node
 * 
 * @param start Starting position in token list
 * @param end Ending position in token list (exclusive)
 * @return t_ast_node* Command AST node, or NULL on failure
 */
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
