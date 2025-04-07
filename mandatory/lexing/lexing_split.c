/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:11:20 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/07 14:55:56 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static void	ft_jump_next(char const *s, char const c, int *i)
{
	while (s[*i] != c && s[*i])
	{
		(*i)++;
		if (s[*i] == '\\' && s[*i + 1] == c)
			*i += 2;
	}
}

static inline int	ft_count_token(char const *s)
{
	int	count;
	int	i;

	if (!s)
		return (-10);
	count = 0;
	i = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && !ft_isspace(s[i]))
				i++;
		}
		ft_jump_next(s, '"', &i);
		ft_jump_next(s, '\'', &i);
	}
	return (count);
}

static inline char	**ft_arr_maker(char const *s, int size)
{
	char	**str;

	if (!s)
		return (NULL);
	str = malloc(sizeof(char *) * (size + 1));
	if (!str)
		return (NULL);
	str[size] = NULL;
	return (str);
}

char	**ft_lexing_split(char *s)
{
	char	**split;
	int		i;
	int		start;
	int		size;

	split = ft_arr_maker(s, ft_count_token(s));
	if (!s || !(split))
		return (NULL);
	i = 0;
	size = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		if (s[i])
		{
			start = i;
			while (s[i] && !ft_isspace(s[i]))
				i++;
			split[size++] = ft_substr(s, start, i - start);
			if (!split[size - 1])
				return (ft_leak(split, size - 1));
		}
	}
	return (split);
}

int main(int argc, char **argv)
{
	char	**split;

	split = ft_lexing_split(argv[1]);
	for (int i = 0; split[i]; i++)
		ft_printerr("%s\n", split[i]);
	ft_printerr("OK\n");
	ft_free_vctr((void **) split);
	return (0);
}