/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:22:45 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/11 17:47:06 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static t_token_type	ft_getiteration(t_token_type type, int *i, int add)
{
	if (add == -1)
		return (TOKEN_ERROR);
	*i += add;
	return (type);
}

static t_token_type	ft_isdouble(char *s, int *i)
{
	if (s[*i] == '|' && s[*i + 1] == '|')
		return (ft_getiteration(TOKEN_OR, i, 2));
	else if (s[*i] == '|')
		return (ft_getiteration(TOKEN_PIPE, i, 1));
	else if (s[*i] == '<' && s[*i + 1] == '<')
		return (ft_getiteration(TOKEN_HEREDOC, i, 2));
	else if (s[*i] == '<')
		return (ft_getiteration(TOKEN_REDIR_IN, i, 1));
	else if (s[*i] == '>' && s[*i + 1] == '>')
		return (ft_getiteration(TOKEN_APPEND, i, 2));
	else if (s[*i] == '>')
		return (ft_getiteration(TOKEN_REDIR_OUT, i, 1));
	else if (s[*i] == '$' && s[*i + 1] == '?')
		return (ft_getiteration(TOKEN_STATUS, i, 2));
	else if (s[*i] == '$')
		return (ft_getiteration(TOKEN_VAR, i, 1));
	else if (s[*i] == '&' && s[*i + 1] == '&')
		return (ft_getiteration(TOKEN_AND, i, 2));
	return (ft_getiteration(TOKEN_ERROR, i, 1));
}

static bool	ft_ismeta(char *meta, char c)
{
	while (*meta)
	{
		if (c == *meta)
			return (true);
		meta++;
	}
	return (false);
}

static int	ft_quote(char *s, char c, bool *quote)
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			if (i > 0 && s[i - 1] == '\\')
				i++;
			else 
				*quote = true;
		}
		if (*quote)
			return (i + 1);
		i++;
	}
	return (-1);
}

static int	ft_end_of_word(char *s, char c)
{
	int	i;

	// ft_printerr("bool: %d, c: %c\n", *quote, c);
	i = -1;
	while (s[++i])
	{
		if (c == ' ')
		{
			while (ft_isspace(s[i]))
				i++;
			i--;
			break ;
		}
		if (ft_ismeta("|><&$\"'()*=\\", s[i]))
			return (i + 1);
		if (s[i] == ' ')
			return (i + 1);
	}
	if (!s[i])
		return (i + 1);
	return (i + 2);
}

t_token_type	ft_get_type(char *s, int *i, bool *quote)
{
	if (s[*i] == '|' || s[*i] == '&' || s[*i] == '<'
		|| s[*i] == '>' || s[*i] == '$')
		return (ft_isdouble(s, i));
	else if (s[*i] == '(')
		return (ft_getiteration(TOKEN_PAREN_OPEN, i, 1));
	else if (s[*i] == '\'')
		return (ft_getiteration(TOKEN_QUOTE, i, 
				ft_quote(&s[*i + 1], s[*i], quote)));
	else if (s[*i] == '"')
		return (ft_getiteration(TOKEN_DQUOTE, i, 
				ft_quote(&s[*i + 1], s[*i], quote)));
	else if (s[*i] == '=')
		return (ft_getiteration(TOKEN_ASSIGN, i, 1));
	else if (s[*i] == ')')
		return (ft_getiteration(TOKEN_PAREN_CLOSE, i, 1));
	else if (s[*i] == '*')
		return (ft_getiteration(TOKEN_WILDCARD, i, 1));
	else if (!s[*i])
		return (ft_getiteration(TOKEN_EOF, i, 0));
	else if (s[*i] == ' ')
		return (ft_getiteration(TOKEN_SPACE, i,
				ft_end_of_word(&s[*i + 1], ' ')));
	else if (s[*i] == '\\')
		return (ft_getiteration(TOKEN_BSLASH, i, 1));
	return (ft_getiteration(TOKEN_WORD, i, ft_end_of_word(&s[*i + 1], 'c')));
}
