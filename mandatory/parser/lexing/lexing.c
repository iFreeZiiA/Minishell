/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 13:56:10 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static int	ft_new_token(char *s, t_list **tok_h, int *i,
	bool *quote)
{
	t_content	tok;
	int			start;

	if (*quote)
	{
		(*i)++;
		*quote = false;
	}
	start = *i;
	tok.token = (t_token *) malloc(sizeof(t_token));
	if (!tok.token)
		return (-1);
	tok.token->type = ft_get_type(s, i, quote);
	if (tok.token->type == TOKEN_ERROR)
		return (-2);
	if (tok.token->type == TOKEN_DQUOTE || tok.token->type == TOKEN_QUOTE)
		start++;
	tok.token->value = ft_substr(s, start, *i - start);
	if (!tok.token->value)
		return (-1);
	if (!ft_lstadd_back(tok_h, tok, TYPE_TOKEN))
		return (-1);
	return (0);
}

int	ft_lexing(t_shell *shell)
{
	t_list	*tok_h;
	char	*line;
	bool	quote;
	int		i;
	int		status;

	tok_h = NULL;
	line = shell->current_line;
	quote = false;
	i = 0;
	while (line[i])
	{
		status = ft_new_token(line, &tok_h, &i, &quote);
		if (status)
			return (status);
	}
	shell->token = tok_h;
	return (0);
}
