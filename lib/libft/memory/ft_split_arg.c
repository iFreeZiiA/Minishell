/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 08:32:52 by jjorda            #+#    #+#             */
/*   Updated: 2024/11/10 13:57:06 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
 * Creates an array of strings based on the input string and delimiter.
 * 
 * @param s The input string to split.
 * @param c The delimiter character.
 * @return A pointer to the array of strings, or NULL if allocation fails.
 */
static inline char	**ft_arrmaker(char const *s)
{
	char	**str;
	int		size_arr;

	if (!s)
		return (NULL);
	size_arr = ft_count_word(s);
	str = (char **) malloc((size_arr + 1) * sizeof(char *));
	if (!str)
		return ((char **) ft_mallocfailed("ft_split"));
	str[size_arr] = NULL;
	return (str);
}

/**
 * Iterates through the input string to find the length of the next substring.
 * 
 * @param s The input string.
 * @param i The current index in the string.
 * @param b_pass pass all the caracters.
 * @param c The delimiter character.
 * @return The length of the next substring.
 */
static int	ft_splitereate(char const *s, int *i, int b_pass)
{
	int	end;

	end = 0;
	if (b_pass == 0)
	{
		if (s[*i] == '\'' || s[*i] == '"')
		{
			end += ft_find_quotes(&s[*i]) + 1;
			*i += end;
			return (end);
		}
		while (s[*i] && s[*i] != ' ')
		{
			end++;
			(*i)++;
		}
		return (end);
	}
	while (s[*i] && ft_isspace(s[*i]))
		(*i)++;
	return (*i);
}

static char	*ft_removequotes(char *s)
{
	char	*temp;
	char	*result;
	int		i;

	i = 0;
	if (s[i] != '\'' && s[i] != '"')
		return (s);
	temp = ft_substr(s, 1, ft_strlen(s) - 2);
	if (s[0] == '\'')
	{
		// Marquer les chaînes qui étaient entourées de guillemets simples
		result = ft_strjoin("\x01", temp);
		free(temp);
		free((void *) s);
		// DEBUG
		// write(2, "DEBUG: Marked single quoted string: ", 37);
		// write(2, result, ft_strlen(result));
		// write(2, "\n", 1);
		return (result);
	}
	free((void *) s);
	return (temp);
}

/**
 * Splits a string into an array of substrings based on the specified delimiter
 * character.
 * 
 * @param s The string to split.
 * @param c The delimiter character.
 * @return A pointer to an array of strings (substrings), or NULL if malloc
 * fails or invalid parameters are provided.
 * 
 * Note: Memory for the array and substrings is allocated using malloc().
 *       If malloc fails, the function returns NULL and cleans up the previously
 * allocated memory.
 */
char	**ft_split_arg(char const *s)
{
	char	**str;
	int		start;
	int		i;
	int		size;

	i = -1;
	size = 0;
	str = ft_arrmaker(s);
	if (!str || !s)
		return (NULL);
	while (s[++i])
	{
		ft_splitereate(s, &i, 1);
		if (!s[i])
			break ;
		start = i;
		str[size] = ft_substr(s, start, ft_splitereate(s, &i, 0));
		str[size] = ft_removequotes(str[size]);
		if (!str[size])
			return (ft_leak(str, size), NULL);
		if (!s[i])
			break ;
		size++;
	}
	return (str);
}
