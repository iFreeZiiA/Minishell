/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:22:45 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/09 16:56:20 by jjorda           ###   ########.fr       */
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
static t_token_type	ft_getite(t_token_type type, int *i, int add)
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
	{
		// ft_printerr("ISDOUBLE0 ||\n");
		return (ft_getite(TOKEN_OR, i, 2));
	}
	else if (s[*i] == '|')
	{
		// ft_printerr("ISDOUBLE1 |\n");
		return (ft_getite(TOKEN_PIPE, i, 1));
	}
	else if (s[*i] == '<' && s[*i + 1] == '<')
	{
		// ft_printerr("ISDOUBLE2 <<\n");
		return (ft_getite(TOKEN_HEREDOC, i, 2));
	}
	else if (s[*i] == '<')
	{
		// ft_printerr("ISDOUBLE3 <\n");
		return (ft_getite(TOKEN_REDIR_IN, i, 1));
	}
	else if (s[*i] == '>' && s[*i + 1] == '>')
	{
		// ft_printerr("ISDOUBLE4 >>\n");
		return (ft_getite(TOKEN_APPEND, i, 2));
	}
	else if (s[*i] == '>')
	{
		// ft_printerr("ISDOUBLE5 >\n");
		return (ft_getite(TOKEN_REDIR_OUT, i, 1));
	}
	else if (s[*i] == '$' && s[*i + 1] == '?')
	{
		// ft_printerr("ISDOUBLE6 $?\n");
		return (ft_getite(TOKEN_STATUS, i, 2));
	}
	else if (s[*i] == '$')
	{
		// ft_printerr("$: %d\n", *i);
		// ft_printerr("ISDOUBLE7 $\n");
		return (ft_getite(TOKEN_VAR, i, 1));
	}
	else if (s[*i] == '&' && s[*i + 1] == '&')
	{
		// ft_printerr("ISDOUBLE8 &&\n");
		return (ft_getite(TOKEN_AND, i, 2));
	}
	// ft_printerr("ISDOUBLE9 OTHER\n");
	return (ft_getite(TOKEN_ERROR, i, 1));
}

/**
 * @brief Finds the end of a quoted string
 * 
 * @param s The input string
 * @param c The quote character (' or ")
 * @param quote Flag to be set if a closing quote is found
 * @return int Position of the closing quote, or -1 if not found
 */
// static int	ft_quote(char *s, char c, bool *quote)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == c)
// 		{
// 			if (i > 0 && s[i - 1] == '\\')
// 				i++;
// 			else
// 			{
// 				*quote = true;
// 				return (i);
// 			}
// 		}
// 		i++;
// 	}
// 	return (-1);
// }

/**
 * @brief Finds the end of a word token
 * 
 * @param s The input string
 * @param c The delimiter character
 * @return int Position of the end of the word
 */
static int	ft_eow(char *s, int c)
{
	int	i;

	i = 0;
	if (c == ' ')
	{
		while (ft_isspace(s[i]))
			i++;
		return (i);
	}
	if (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
	{
		while (s[i] && (ft_isalnum(s[i]) || s[i] == '_') && s[i] != ' ')
			i++;
		return (i);
	}
	if (!s[i])
		return (i);
	while (s[i] && !ft_ismeta("|><$\"'()*=\\", s[i]) && s[i] != ' ')
		i++;
	return (i);
}

static t_token_type	ft_allquote(char *s, int *i, bool *quote, char c)
{
	int	y;
	int	end;

	y = 0;
	while (s[*i + ++y])
	{
		if (s[*i + y] == c)
		{
			if (y > 0 && s[*i + y - 1] == '\\')
				y++;
			else
			{
				*quote = true;
				break ;
			}
		}
	}
	// quote_end = ft_quote(&s[*i + 1], s[*i], quote);
	end = -1;
	if (y != -1)
		end = y;
	if (c == '"')
		return (ft_getite(TOKEN_DQUOTE, i, end));
	return (ft_getite(TOKEN_QUOTE, i, end));
}
// static int	ft_quote(char *s, char c, bool *quote)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == c)
// 		{
// 			if (i > 0 && s[i - 1] == '\\')
// 				i++;
// 			else
// 			{
// 				*quote = true;
// 				return (i);
// 			}
// 		}
// 		i++;
// 	}
// 	return (-1);
// }
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
	{
		// ft_printerr("GET_TYPE0 EOF\n");
		return (ft_getite(TOKEN_EOF, i, 0));
	}
	if (ft_ismeta("|<>$", s[*i]) || (s[*i] == '&' && s[*i + 1] == '&'))
	{
		// ft_printerr("GET_TYPE1 DOUBLE\n");
		return (ft_isdouble(s, i));
	}
	else if (s[*i] == '(')
	{
		// ft_printerr("GET_TYPE2 (\n");
		return (ft_getite(TOKEN_PAREN_OPEN, i, 1));
	}
	else if (s[*i] == ')')
	{
		// ft_printerr("GET_TYPE3 )\n");
		return (ft_getite(TOKEN_PAREN_CLOSE, i, 1));
	}
	else if (s[*i] == '*')
	{
		// ft_printerr("GET_TYPE4 *\n");
		return (ft_getite(TOKEN_WILDCARD, i, 1));
	}
	else if (s[*i] == '=')
	{
		// ft_printerr("GET_TYPE5 =\n");
		return (ft_getite(TOKEN_ASSIGN, i, 1));
	}
	else if (s[*i] == '\\')
	{
		// ft_printerr("GET_TYPE6 \\\n");
		return (ft_getite(TOKEN_BSLASH, i, 1));
	}
	else if (s[*i] == '\'')
	{
		// ft_printerr("GET_TYPE7 '\n");
		return (ft_allquote(s, i, quote, '\''));
	}
	else if (s[*i] == '"')
	{
		// ft_printerr("GET_TYPE8 \"\n");
		return (ft_allquote(s, i, quote, '"'));
	}
	else if (ft_isspace(s[*i]))
	{
		// ft_printerr("GET_TYPE9 ' '\n");
		return (ft_getite(TOKEN_SPACE, i, ft_eow(&s[*i + 1], ' ') + 1));
	}
	// ft_printerr("GET_TYPE10 WORD\n");
	return (ft_getite(TOKEN_WORD, i, ft_eow(&s[*i], 'w')));
}
