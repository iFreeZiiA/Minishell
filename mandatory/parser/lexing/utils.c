/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:22:45 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 18:21:27 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Updates the iterator and returns the token type
 * 
 * @param type The token type to return
 * @param i Pointer to the current position in the string
 * @param add Number of positions to advance
 * @return t_token_type The token type, or TOKEN_ERROR on error
 */
static t_token_type	ft_getiteration(t_token_type type, int *i, int add)
{
	if (add == -1)
		return (TOKEN_ERROR);
	*i += add;
	return (type);
}

/**
 * @brief Identifies double character tokens like ||, >>, <<, etc.
 * 
 * @param s The input string
 * @param i Pointer to the current position
 * @return t_token_type The identified token type
 */
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

/**
 * @brief Finds the end of a quoted string
 * 
 * @param s The input string
 * @param c The quote character (' or ")
 * @param quote Flag to be set if a closing quote is found
 * @return int Position of the closing quote, or -1 if not found
 */
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
			{
				*quote = true;
				return (i + 1);
			}
		}
		i++;
	}
	return (-1);
}

/**
 * @brief Finds the end of a word token
 * 
 * @param s The input string
 * @param c The delimiter character
 * @return int Position of the end of the word
 */
static int	ft_end_of_word(char *s, int c)
{
	int	i;

	i = 0;
	/* Handle whitespace */
	if (c == ' ')
	{
		while (ft_isspace(s[i]))
			i++;
		return (i);
	}
	
	/* Skip alphanumeric characters and underscores */
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	
	/* Check for meta characters */
	if (s[i] && ft_ismeta("|><&$\"'()*=\\", s[i]))
		return (i);
	
	/* Handle end of string */
	if (!s[i])
		return (i);
	
	return (i);
}

/**
 * @brief Identifies the token type at the current position
 * 
 * @param s The input string
 * @param i Pointer to the current position
 * @param quote Flag to track quote status
 * @return t_token_type The identified token type
 */
t_token_type	ft_get_type(char *s, int *i, bool *quote)
{
	if (!s[*i])
		return (ft_getiteration(TOKEN_EOF, i, 0));
	
	/* Check special characters */
	if (ft_ismeta("|&<>$", s[*i]))
		return (ft_isdouble(s, i));
	else if (s[*i] == '(')
		return (ft_getiteration(TOKEN_PAREN_OPEN, i, 1));
	else if (s[*i] == ')')
		return (ft_getiteration(TOKEN_PAREN_CLOSE, i, 1));
	else if (s[*i] == '*')
		return (ft_getiteration(TOKEN_WILDCARD, i, 1));
	else if (s[*i] == '=')
		return (ft_getiteration(TOKEN_ASSIGN, i, 1));
	else if (s[*i] == '\\')
		return (ft_getiteration(TOKEN_BSLASH, i, 1));
	
	/* Handle quotes */
	else if (s[*i] == '\'')
	{
		int quote_end = ft_quote(&s[*i + 1], s[*i], quote);
		return (ft_getiteration(TOKEN_QUOTE, i, quote_end == -1 ? -1 : quote_end + 1));
	}
	else if (s[*i] == '"')
	{
		int quote_end = ft_quote(&s[*i + 1], s[*i], quote);
		return (ft_getiteration(TOKEN_DQUOTE, i, quote_end == -1 ? -1 : quote_end + 1));
	}
	
	/* Handle whitespace */
	else if (ft_isspace(s[*i]))
	{
		int space_end = ft_end_of_word(&s[*i + 1], ' ');
		return (ft_getiteration(TOKEN_SPACE, i, space_end + 1));
	}
	
	/* Handle regular words */
	else
	{
		int word_end = ft_end_of_word(&s[*i], 'w');
		return (ft_getiteration(TOKEN_WORD, i, word_end));
	}
}