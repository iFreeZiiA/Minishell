/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/08 16:54:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static t_token_type	ft_getiteration(t_token_type type, int *i, int add)
{
	*i += add;
	return (type);
}

static t_token_type ft_isdouble(char *s, int *i)
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
	// if (c == '|' || c == '>' || c == '<' || c == '&' || c == '$' || c == '"' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '*' || c == '=')
}

static int	ft_end_of_word(char *s, char c)
{
	int	i;

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
		if (ft_ismeta("|><&$\"'()*=", s[i]))
			return (i + 1);
		if (s[i] == ' ')
			return (i + 1);
	}
	if (!s[i])
		return (i + 1);
	return (i + 2);
}

// int	main(int argc, char **argv)
// {
// 	if (argv[1][0] != '"' && argv[1][0] != '\'')
// 		ft_printerr("%d\n", ft_end_of_word(argv[1], argv[2][0]));
// 	else
// 		ft_printerr("%d\n", ft_end_of_word(argv[1] + 1, argv[2][0]));
// 	return (0);
// }

static t_token_type ft_get_type(char *s, int *i)
{
	if (s[*i] == '|' || s[*i] == '&' || s[*i] == '<' || s[*i] == '>' || s[*i] == '$')
		return (ft_isdouble(s, i));
	else if (s[*i] == '(')
		return (ft_getiteration(TOKEN_PAREN_OPEN, i, 1));
	else if (s[*i] == '\'')
		return (ft_getiteration(TOKEN_QUOTE, i, 1));
	else if (s[*i] == '"')
		return (ft_getiteration(TOKEN_DQUOTE, i, 1));
	else if (s[*i] == '=')
		return (ft_getiteration(TOKEN_ASSIGN, i, 1));
	else if (s[*i] == ')')
		return (ft_getiteration(TOKEN_PAREN_CLOSE, i, 1));
	else if (s[*i] == '*')
		return (ft_getiteration(TOKEN_WILDCARD, i, 1));
	else if (!s[*i])
		return (ft_getiteration(TOKEN_EOF, i, 0));
	else if (s[*i] == ' ')
		return (ft_getiteration(TOKEN_SPACE, i, ft_end_of_word(&s[*i + 1], ' ')));
	return (ft_getiteration(TOKEN_WORD, i, ft_end_of_word(&s[*i + 1], 'c')));
}

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


static void	*ft_tok_err(int *p_status, int status)
{
	*p_status = status;
	return (NULL);
}

static t_list	*ft_new_token(char *s, t_list **tok_h, int *i, int *status)
{
	t_token	*tok;
	int		start;

	// sleep(1);
	*status = 0;
	tok = (t_token *) malloc(sizeof(t_token));
	if (!tok)
		return (ft_tok_err(status, -1));
	start = *i;
	tok->type = ft_get_type(s, i);
	if (tok->type == TOKEN_ERROR)
		return (ft_tok_err(status, -2));
	// if (tok->type == TOKEN_DQUOTE || tok->type == TOKEN_QUOTE)
	// 	tok->value = ft_substr(s, start + 1, (*i - start) - 2);
	// else
		tok->value = ft_substr(s, start, *i - start);
	if (!tok->value)
		return (ft_tok_err(status, -1));
	ft_printerr("CURRENT: '%s': %d\n", tok->value, tok->type);
	if (!ft_lstadd_back(tok_h, (void *) tok))
		return (ft_tok_err(status, -1));
	return (*tok_h);
}

void	*ft_lstfree_t(t_list *tok_h)
{
	t_token	*tok;
	t_list	*current;

	if (!tok_h)
		return (NULL);
	ft_lstiter(tok_h, free);
	current = tok_h;
	while (current)
	{
		tok = (t_token *) current->content;
		free(tok->value);
		current = tok_h->next;
		free(tok_h);
		tok_h = NULL;
	}
	return (NULL);
}

void	ft_print_list(t_list *tok_h)
{
	t_list	*tok_curr;
	t_token	*tok;

	tok_curr = tok_h;
	while (tok_curr)
	{
		tok = (t_token *) tok_curr->content;
		ft_printerr("%s: %d\n", tok->value, tok->type);
		tok_curr = tok_curr->next;
	}
}

int	ft_lexing(t_shell *shell)
{
	t_list	*tok_h;
	t_token	*tok;
	char	*line;
	int		i;
	int		status;

	tok_h = NULL;
	// ft_printerr("PING lexing 0\n");
	line = shell->current_line;
	i = 0;
	while (line[i])
	{
		ft_printerr("before: %c\n", line[i]);
		ft_printerr("after: %c\n", line[i]);
		ft_new_token(line, &tok_h, &i, &status);
		// ft_printerr("PING lexing 1\n");
		if (status)
			return (status);
		// ft_printerr("PING lexing 2\n");
	}
	
	ft_print_list(tok_h);
	// ft_printerr("PING lexing 3\n");
	// ft_lstfree_t(tok_h);
	// ft_printerr("PING lexing 4\n");
	return (0);
}

int	main(int argc, char **argv)
{
	t_shell	shell;
	int		res;

	shell.current_line = argv[1];
	res = ft_lexing(&shell);
	if (res)
	{
		if (res == -1)
			ft_printerr("MALLOC ERRRO\n");
		if (res == -2)
			ft_printerr("WRITE ERROR\n");
		ft_printerr("%d\n", res);
		return (res);
	}
	return (0);
}
