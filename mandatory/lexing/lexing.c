/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/07 14:34:33 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static token_type ft_get_type(char *s)
{
	if (s[0] == '|' && s[1] == '|' && !s[2])
		return (TOKEN_OR);
	else if (s[0] == '&' && s[1] == '&' && !s[2])
		return (TOKEN_AND);
	else if (s[0] == '<' && s[1] == '<' && !s[2])
		return (TOKEN_HEREDOC);
	else if (s[0] == '>' && s[1] == '>' && !s[2])
		return (TOKEN_APPEND);
	else if (s[0] == '|' && !s[1])
		return (TOKEN_PIPE);
	else if (s[0] == '<' && !s[1])
		return (TOKEN_REDIR_IN);
	else if (s[0] == '>' && !s[1])
		return (TOKEN_REDIR_OUT);
	else if (s[0] == '(' && !s[1])
		return (TOKEN_PAREN_OPEN);
	else if (s[0] == ')' && !s[1])
		return (TOKEN_PAREN_CLOSE);
	else if (s[0] == '$' && !s[1])
		return (TOKEN_DOLLAR);
	else if (s[0] == '*' && !s[1])
		return (TOKEN_WILDCARD);
	else if (!s[0])
		return (TOKEN_EOF);
	return (TOKEN_WORD);
}

// static bool	ft_new_token(t_list **token, char **vec, int i)
// {
// 	t_token	*tok;
// 	int		i;

	
// 	return (true);
// }



static inline bool	ft_tokenisation(t_shell *shell, t_list **token, char *exp)
{
	t_token	*tok;

	tok = (t_token *) malloc(sizeof(t_token));
	if (!tok)
		return (false);
	tok->value = exp;
	tok->type = ft_get_type(exp);
	if (!ft_lstadd_back(token, (void *) tok))
		return (false);
	
}

static int	ft_isspecial_char(char c)
{
	if (c == '&' || c == '|' || c == '<' || c =='>' || c == '(' || c == ')')
		return (true);
	return (false);
}

static t_list	*ft_lexing_split(t_shell *shell, char *input)
{
	t_list	*tok_h;
	bool	quote;
	bool	is_esc;
	int		i;

	tok_h = (t_list *) malloc(sizeof(t_list));
	quote = false;
	i = -1;
	while (input[++i])
	{
		if (!quote)
		{
			while (ft_isspace(input[i]))
				i++;
			if (ft_isspecial_char(input[i]))
				ft_get_operator(input[i], input, i, &tok_h);
			else if (ft_isquote(input[i]))
				quote = true;
			else if (input[i] == '$')
				ft_getvar();
			else
				ft_getword();
		}
		else
		{
			if (input[i -1] == '"')
				is_esc = true;
			ft_gettoken(tok_h, input, input[i -1], is_esc);
			quote = false;
			is_esc = false;
		}
	}
}

int	ft_lexing(t_shell *shell)
{
	t_list	*tok_h;

	tok_h = ft_lexing_split(shell, shell->current_line);
	if (!tok_h)
		return (-1);
	// if (ft_tokenisation(shell, &shell->token, split))
	// 	ft_lexing_err(shell, split);
}