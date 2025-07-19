/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 14:31:19 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Matches pattern against string with wildcard support
 * 
 * @param pattern Pattern with wildcards
 * @param string String to match
 * @return bool true if match found
 */
bool	ft_match_pattern(const char *pattern, const char *string)
{
	const char	*p;
	const char	*s;
	const char	*star_p;
	const char	*star_s;

	p = pattern;
	s = string;
	star_p = NULL;
	star_s = NULL;
	while (*s)
	{
		if (*p == '*')
		{
			star_p = ++p;
			star_s = s;
		}
		else if (*p == *s)
		{
			p++;
			s++;
		}
		else
		{
			ft_reset_pattern_match(&p, &s, star_p, &star_s);
			if (!p)
				return (false);
		}
	}
	return (ft_check_remaining_pattern(p));
}

/**
 * @brief Resets pattern matching to last star position
 * 
 * @param p Pattern pointer
 * @param s String pointer
 * @param star_p Star pattern position
 * @param star_s Star string position
 */
void	ft_reset_pattern_match(const char **p, const char **s,
			const char *star_p, const char **star_s)
{
	if (!star_p)
	{
		*p = NULL;
		*s = NULL;
		return ;
	}
	*p = star_p;
	(*star_s)++;
	*s = *star_s;
}

/**
 * @brief Checks if remaining pattern is valid
 * 
 * @param p Pattern pointer
 * @return bool true if pattern fully matched
 */
bool	ft_check_remaining_pattern(const char *p)
{
	while (*p == '*')
		p++;
	return (*p == '\0');
}

/**
 * @brief Determines if hidden files should be shown
 * 
 * @param filename Filename to check
 * @param pattern Pattern being matched
 * @return bool true if should show hidden
 */
bool	ft_should_show_hidden(const char *filename, const char *pattern)
{
	if (!filename || !pattern)
		return (false);
	if (filename[0] != '.')
		return (true);
	if (pattern[0] == '.')
		return (true);
	return (false);
}

/**
 * @brief Sorts array of matches alphabetically
 * 
 * @param arr Array of strings
 * @param size Size of array
 */
void	ft_sort_matches(char **arr, int size)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
