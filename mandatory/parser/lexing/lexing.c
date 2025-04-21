/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/21 18:54:49 by jjorda           ###   ########.fr       */
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

t_list	*ft_lexing(t_shell *shell, char *prompt, int *status)
{
	t_list	*tok_h;
	bool	quote;
	int		i;

	tok_h = NULL;
	// ft_printerr("Ping ft_lexing 0\n");
	quote = false;
	i = 0;
	while (prompt[i])
	{
		// ft_printerr("Ping ft_lexing 1\n");
		*status = ft_new_token(prompt, &tok_h, &i, &quote);
	}
	// ft_printerr("%s:%d\n", tok_h->next->content.token->value, tok_h->next->content.token->type);
	ft_expansion(shell, tok_h, status);
	// if (shell->token)
	ft_printerr("Ping ft_lexing 1\n");
	ft_print_list(tok_h);
	ft_printerr("Ping ft_lexing 2\n");
	return (tok_h);
}
