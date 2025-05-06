// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   type.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/04/08 18:22:45 by jjorda            #+#    #+#             */
// /*   Updated: 2025/05/06 19:06:03 by jjorda           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../../header/minishell.h"

// /**
//  * @brief Updates the iterator and returns the token type
//  * 
//  * @param type The token type to return
//  * @param i Pointer to the current position in the string
//  * @param add Number of positions to advance
//  * @return t_token_type The token type, or TOKEN_ERROR on error
//  */
// t_token_type	ft_getiteration(t_token_type type, int *i, int add)
// {
// 	if (add == -1)
// 		return (TOKEN_ERROR);
// 	*i += add;
// 	return (type);
// }

// /**
//  * @brief Identifies double character tokens like ||, >>, <<, etc.
//  * 
//  * @param s The input string
//  * @param i Pointer to the current position
//  * @return t_token_type The identified token type
//  */
// static t_token_type	ft_isdouble(char *s, int *i)
// {
// 	if (s[*i] == '|' && s[*i + 1] == '|')
// 		return (ft_getiteration(TOKEN_OR, i, 2));
// 	else if (s[*i] == '|')
// 		return (ft_getiteration(TOKEN_PIPE, i, 1));
// 	else if (s[*i] == '<' && s[*i + 1] == '<')
// 		return (ft_getiteration(TOKEN_HEREDOC, i, 2));
// 	else if (s[*i] == '<')
// 		return (ft_getiteration(TOKEN_REDIR_IN, i, 1));
// 	else if (s[*i] == '>' && s[*i + 1] == '>')
// 		return (ft_getiteration(TOKEN_APPEND, i, 2));
// 	else if (s[*i] == '>')
// 		return (ft_getiteration(TOKEN_REDIR_OUT, i, 1));
// 	else if (s[*i] == '$' && s[*i + 1] == '?')
// 		return (ft_getiteration(TOKEN_STATUS, i, 2));
// 	else if (s[*i] == '$')
// 		return (ft_getiteration(TOKEN_VAR, i, 1));
// 	else if (s[*i] == '&' && s[*i + 1] == '&')
// 		return (ft_getiteration(TOKEN_AND, i, 2));
// 	return (ft_getiteration(TOKEN_ERROR, i, 1));
// }

// /**
//  * @brief Identifies the token type at the current position
//  * 
//  * @param s The input string
//  * @param i Pointer to the current position
//  * @param quote Flag to track quote status
//  * @return t_token_type The identified token type
//  */
// t_token_type	ft_get_type(char *s, int *i, bool *quote)
// {
// 	if (!s[*i])
// 		return (ft_getiteration(TOKEN_EOF, i, 0));
// 	if (ft_ismeta("|&<>$", s[*i]))
// 		return (ft_isdouble(s, i));
// 	else if (s[*i] == '(')
// 		return (ft_getiteration(TOKEN_PAREN_OPEN, i, 1));
// 	else if (s[*i] == ')')
// 		return (ft_getiteration(TOKEN_PAREN_CLOSE, i, 1));
// 	else if (s[*i] == '*')
// 		return (ft_getiteration(TOKEN_WILDCARD, i, 1));
// 	else if (s[*i] == '=')
// 		return (ft_getiteration(TOKEN_ASSIGN, i, 1));
// 	else if (s[*i] == '\\')
// 		return (ft_getiteration(TOKEN_BSLASH, i, 1));
// 	else if (s[*i] == '\'')
// 		return (ft_getend(s, i, quote, false));
// 	else if (s[*i] == '"')
// 		return (ft_getend(s, i, quote, false));
// 	else if (ft_isspace(s[*i]))
// 		return (ft_getend(s, i, NULL, true));
// 	else
// 		return (ft_getend(s, i, NULL, true));
// }
