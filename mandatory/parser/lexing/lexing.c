/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 15:38:12 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static int	ft_new_token(char *s, t_list **tok_h, int *i,
	bool *quote)
{
	t_content	tok;
	int			start;
	int			end;

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
	end = *i - start;
	if (*tok_h)
	{
		ft_printerr("PING new_token 1\n");
		t_list *lst = ft_lstlast(*tok_h);
		// if (!lst)
			// ft_printerr("ERTROR\n");
		ft_printerr("%s\n", lst->content.token->value);
		if (lst->content.token->type == TOKEN_VAR)
		{
			end = ft_eov(&s[start]);
			ft_printerr("PINK: %d\n", end);
		}
	}
	// ft_printerr("PING new_token 2\n");
	tok.token->value = ft_substr(s, start, end);
	if (!tok.token->value)
		return (-1);
	if (!ft_lstadd_back(tok_h, tok, TYPE_TOKEN))
		return (-1);
	return (0);
}

t_list	*ft_lexing(t_shell *shell)
{
	t_list	*tok_h;
	bool	quote;
	int		status;
	int		i;

	if (!shell)
		return (NULL);
	tok_h = NULL;
	// ft_printerr("Ping ft_lexing 0\n");
	quote = false;
	i = 0;
	while (shell->current_line[i])
	{
		// ft_printerr("Ping ft_lexing 1\n");
		status = ft_new_token(shell->current_line, &tok_h, &i, &quote);
	}
	// ft_printerr("%s:%d\n", tok_h->next->content.token->value, tok_h->next->content.token->type);
	ft_print_list(tok_h);
	ft_printerr("\n");
	shell->env->local_env = malloc(sizeof(char *) * 2);
	shell->env->local_env[0] = "VAR=ok";
	shell->env->local_env[1] = NULL;
	if (ft_expansion(shell, tok_h, &status) == -1)
		return (NULL);
	if (!shell->token)
		shell->token = tok_h;
	// ft_printerr("tok_h: %s\n", shell->token->content.token->value);
	// ft_printerr("Ping ft_lexing 2\n");
	return (tok_h);
}
