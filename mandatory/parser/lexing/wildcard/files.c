/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/27 15:45:11 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Gets all files matching pattern
 * 
 * @param pattern Pattern to match
 * @return char** Array of matching filenames
 */
char	**ft_get_matching_files(const char *pattern)
{
	DIR		*dir;
	char	**matches;
	int		capacity;
	int		count;
	char	**result;

	dir = opendir(".");
	result = NULL;
	if (dir)
	{
		capacity = 100;
		matches = malloc(sizeof(char *) * capacity);
		if (matches)
		{
			count = ft_collect_matches(dir, pattern, matches, &capacity);
			if (count > 0)
			{
				matches[count] = NULL;
				ft_sort_matches(matches, count);
				result = matches;
			}
			else
				free(matches);
		}
		closedir(dir);
	}
	return (result);
}

/**
 * @brief Collects matching files from directory
 * 
 * @param dir Directory pointer
 * @param pattern Pattern to match
 * @param matches Array to store matches
 * @param capacity Pointer to array capacity
 * @return int Number of matches found
 */
int	ft_collect_matches(DIR *dir, const char *pattern, char **matches,
			int *capacity)
{
	struct dirent	*entry;
	int				count;
	bool			should_add;

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		should_add = false;
		if (ft_should_show_hidden(entry->d_name, pattern))
		{
			if (ft_match_pattern(pattern, entry->d_name))
				should_add = true;
		}
		if (should_add)
		{
			if (!ft_add_match_to_array(matches, &count, capacity, entry->d_name))
				break ;
		}
		entry = readdir(dir);
	}
	return (count);
}

/**
 * @brief Adds match to array, expanding if needed
 * 
 * @param matches Array of matches
 * @param count Pointer to current count
 * @param capacity Pointer to array capacity
 * @param filename Filename to add
 * @return bool true on success
 */
bool	ft_add_match_to_array(char **matches, int *count, int *capacity,
			const char *filename)
{
	char	*new_match;

	if (*count >= *capacity - 1)
		return (false);
	new_match = ft_strdup((char *) filename);
	if (!new_match)
		return (false);
	matches[*count] = new_match;
	(*count)++;
	return (true);
}

/**
 * @brief Connects new tokens to list, replacing current
 * 
 * @param tok_h Token list head
 * @param tok_curr Current token to replace
 * @param new_tokens New tokens to insert
 * @return bool true on success
 */
bool	ft_connect_new_tokens(t_list **tok_h, t_list *tok_curr,
			t_list *new_tokens)
{
	t_list	*prev;
	t_list	*next;
	t_list	*last_new;

	if (!tok_h || !tok_curr || !new_tokens)
		return (false);
	prev = NULL;
	next = tok_curr->next;
	if (*tok_h == tok_curr)
		*tok_h = new_tokens;
	else
	{
		prev = *tok_h;
		while (prev && prev->next != tok_curr)
			prev = prev->next;
		if (prev)
			prev->next = new_tokens;
	}
	last_new = new_tokens;
	while (last_new->next)
		last_new = last_new->next;
	last_new->next = next;
	ft_clean_node_tok(tok_curr);
	return (true);
}

/**
 * @brief Creates token list from matches
 * 
 * @param matches Array of matching filenames
 * @return t_list* Token list
 */
t_list	*ft_create_match_tokens(char **matches)
{
	t_list	*token_list;
	int		count;
	bool	success;

	if (!matches)
		return (NULL);
	count = 0;
	while (matches[count])
		count++;
	token_list = NULL;
	success = ft_build_token_list(matches, count, &token_list);
	if (!success)
		return (NULL);
	return (token_list);
}
