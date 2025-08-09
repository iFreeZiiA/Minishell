/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:45:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 14:45:00 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

char	*ft_handle_simple_quotes(char *word, int len)
{
	char	*temp;
	char	*result;

	if (word[0] == '\'' && word[len - 1] == '\'')
	{
		temp = ft_substr(word, 1, len - 2);
		result = ft_strjoin("\x01", temp);
		free(temp);
		free(word);
		return (result);
	}
	else if (word[0] == '"' && word[len - 1] == '"')
	{
		result = ft_substr(word, 1, len - 2);
		free(word);
		return (result);
	}
	return (NULL);
}

char	*ft_handle_assignment_quotes(char *word, char *equals_pos, int len)
{
	char	*before_quotes;
	char	*quoted_part;
	char	*temp;
	char	*result;

	if (equals_pos[1] == '\'' && word[len - 1] == '\'')
	{
		before_quotes = ft_substr(word, 0, equals_pos - word + 1);
		quoted_part = ft_substr(word, equals_pos - word + 2,
				len - (equals_pos - word + 2) - 1);
		temp = ft_strjoin("\x01", quoted_part);
		result = ft_strjoin(before_quotes, temp);
		free(before_quotes);
		free(quoted_part);
		free(temp);
		free(word);
		return (result);
	}
	return (NULL);
}

char	*ft_handle_double_assignment(char *word, char *equals_pos, int len)
{
	char	*before_quotes;
	char	*quoted_part;
	char	*result;

	if (equals_pos[1] == '"' && word[len - 1] == '"')
	{
		before_quotes = ft_substr(word, 0, equals_pos - word + 1);
		quoted_part = ft_substr(word, equals_pos - word + 2,
				len - (equals_pos - word + 2) - 1);
		result = ft_strjoin(before_quotes, quoted_part);
		free(before_quotes);
		free(quoted_part);
		free(word);
		return (result);
	}
	return (word);
}

char	*ft_process_quoted_word(char *word)
{
	int		len;
	char	*result;
	char	*equals_pos;

	len = ft_strlen(word);
	if (len >= 2)
	{
		result = ft_handle_simple_quotes(word, len);
		if (result)
			return (result);
	}
	equals_pos = ft_strchr(word, '=');
	if (equals_pos)
	{
		result = ft_handle_assignment_quotes(word, equals_pos, len);
		if (result)
			return (result);
		result = ft_handle_double_assignment(word, equals_pos, len);
		return (result);
	}
	return (word);
}
