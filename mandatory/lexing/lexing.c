/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/07 19:00:01 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static token_type	ft_getiteration(token_type type, int *i, int add)
{
	if (add < 0)
		return (TOKEN_ERROR);
	*i += add;
	return (type);
}

static token_type ft_isdouble(char *s, int *i)
{
	if (s[0] == '|' && s[1] == '|')
		return (ft_getiteration(TOKEN_OR, i, 2));
	else if (s[0] == '|')
		return (ft_getiteration(TOKEN_PIPE, i, 1));
	else if (s[0] == '<' && s[1] == '<')
		return (ft_getiteration(TOKEN_HEREDOC, i, 2));
	else if (s[0] == '<')
		return (ft_getiteration(TOKEN_REDIR_IN, i, 1));
	else if (s[0] == '>' && s[1] == '>')
		return (ft_getiteration(TOKEN_APPEND, i, 2));
	else if (s[0] == '>')
		return (ft_getiteration(TOKEN_REDIR_OUT, i, 1));
	else if (s[0] == '$' && s[1] == '?')
		return (ft_getiteration(TOKEN_STATUS, i, 2));
	else if (s[0] == '$')
		return (ft_getiteration(TOKEN_VAR, i, 1));
	else if (s[0] == '&' && s[1] == '&')
		return (ft_getiteration(TOKEN_AND, i, 2));
	return (ft_getiteration(TOKEN_ERROR, i, 1));
}

//Need a change
static int	ft_end_of_word(char *s, char c)
{
	int	i;

	if (s[0] == c && c != ' ')
		return (2);
	i = -1;
	while (s[++i])
	{
		if (c == '\'' || c == '"')
		{
			if (s[i] == c && s[i - 1] == '\\')
				i++;
			else if (s[i] == c)
			{
				ft_printerr("BREAK\n");
				break ;
			}
		}
		if (s[i] == ' ' && c == ' ')
		{
			ft_printerr("BREAK HERE\n");
			return (i + 1);
		}
		if (s[i] == '$' && c == '"')
		{
			ft_printerr("c = %c, i = %d\n", c, i);
			return (i - 1);
		}
		ft_printerr("%c:%d | ", s[i], i);
	}
	if (!s[i] && c != ' ')
		return (-1);
	ft_printerr("\n");
	ft_printerr("%s\n", &s[i]);
	return (i + 1);
}

int	main(int argc, char **argv)
{
	if (argv[1][0] != '"' && argv[1][0] != '\'')
		ft_printerr("%d\n", ft_end_of_word(argv[1], argv[2][0]));
	else
		ft_printerr("%d\n", ft_end_of_word(argv[1] + 1, argv[2][0]));
	return (0);
}

static token_type ft_get_type(char *s, int *i)
{
	if (s[0] == '|' || s[0] == '&' || s[0] == '<' || s[0] == '>' || s[0] == '$')
		return (ft_isdouble(s, i));
	else if (s[0] == '(')
		return (ft_getiteration(TOKEN_PAREN_OPEN, i, 1));
	else if (s[0] == '\'')
		return (ft_getiteration(TOKEN_QUOTE, i, 1));
	else if (s[0] == '"')
		return (ft_getiteration(TOKEN_DQUOTE, i, 1));
	else if (s[0] == '=')
		return (ft_getiteration(TOKEN_ASSIGN, i, 1));
	else if (s[0] == ')')
		return (ft_getiteration(TOKEN_PAREN_CLOSE, i, 1));
	else if (s[0] == '*')
		return (ft_getiteration(TOKEN_WILDCARD, i, 1));
	else if (!s[0])
		return (ft_getiteration(TOKEN_EOF, i, 1));
	return (ft_getiteration(TOKEN_WORD, i, ft_end_of_word(&s[*i + 1], s[*i])));
}

// static bool	ft_new_token(t_list **token, char **vec, int i)
// {
// 	t_token	*tok;
// 	int		i;ft_get_type
	
// 	return (true);
// }


// static inline bool	ft_tokenisation(t_shell *shell, t_list **token, char *exp)
// {
// 	t_token	*tok;

// 	tok = (t_token *) malloc(sizeof(t_token));
// 	if (!tok)
// 		return (false);
// 	tok->value = exp;
// 	tok->type = ft_get_type(exp);
// 	if (!ft_lstadd_back(token, (void *) tok))
// 		return (false);
	
// }

// static int	ft_new_token(t_list	**tok_h, char *s, int *i)
// {
// 	t_token	*tok;

// 	tok = (t_token *) malloc(sizeof(t_token));
// 	if (!tok)
// 		return (-1);
// 	tok->type = ft_get_type(s, i);
// 	if (tok->type == TOKEN_ERROR)
// 		return (-2);
// 	tok->value = ft_strdup(s);
// 	if (!tok->value)
// 		return (-1);
// 	if (!ft_lstadd_back(tok_h, (void *) tok))
// 		return (-1);
// 	return (0);
// }

// // static bool	ft_which_token(t_list **tok_h, char *s)
// // {
// // 	if (s[])
// // }

// static bool	ft_isspecial_char(char c)
// {
// 	if (c == '&' || c == '|' || c == '<' || c =='>' || c == '(' || c == ')')
// 		return (true);
// 	return (false);
// }

// static t_list	*ft_lexing_split(t_shell *shell, char *input)
// {
// 	t_list	*tok_h;
// 	bool	quote;
// 	bool	is_esc;
// 	int		i;

// 	tok_h = (t_list *) malloc(sizeof(t_list));
// 	quote = false;
// 	i = -1;
// 	while (input[++i])
// 	{
// 		if (!quote)
// 		{
// 			while (ft_isspace(input[i]))
// 				i++;
// 			if (ft_isspecial_char(input))
// 				ft_get_operator(input[i], input, i, &tok_h);
// 			else if (ft_isquote(input[i]))
// 				quote = true;
// 			else if (input[i] == '$')
// 				ft_getvar();
// 			else
// 				ft_getword();
// 		}
// 		else
// 		{
// 			if (input[i -1] == '"')
// 				is_esc = true;
// 			ft_gettoken(tok_h, input, input[i -1], is_esc);
// 			quote = false;
// 			is_esc = false;
// 		}
// 	}
// }

// int	ft_lexing(t_shell *shell)
// {
// 	t_list	*tok_h;

// 	tok_h = ft_lexing_split(shell, shell->current_line);
// 	if (!tok_h)
// 		return (-1);
// 	// if (ft_tokenisation(shell, &shell->token, split))
// 	// 	ft_lexing_err(shell, split);
// }