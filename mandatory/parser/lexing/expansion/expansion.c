/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:32 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/29 20:58:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

t_list	*ft_getexpan(t_shell *shell, t_list *tok_h, t_list **tok_c, t_token *tok_n)
{
	t_list	*tok_l;
	t_token	*new_tok;
	char	*str;

	if (!shell || !tok_h || !(*tok_c)->content.token || !tok_n)
		return (NULL);
	if (!(*tok_c)->next || (*tok_c)->next->content.token->type != TOKEN_WORD)
	{
		(*tok_c)->content.token->type = TOKEN_WORD;
		return (*tok_c);
	}
	new_tok = malloc(sizeof(t_token));
	if (!new_tok)
		return (NULL);
	str = (*tok_c)->next->content.token->value;
	ft_printerr("GETEXPAN: '%s': %d\n", str,  ft_strlen(str));
	new_tok->value = ft_getenv_value(shell, str, 0, ft_strlen(str));
	ft_printerr("PING GETEXPAN 1\n");
	if (!new_tok->value)
		return (NULL);
	ft_printerr("PING GETEXPAN 2\n");
	tok_l = ft_lstnew_tok(new_tok);
	ft_printerr("PING GETEXPAN 3\n");
	if (!tok_l)
		return (NULL);
	tok_l->content.token->type = TOKEN_WORD;
	ft_printerr("PING GETEXPAN 4\n");
	if (!ft_lstreplace_n(tok_c, tok_l, ft_clean_node_tok, 2))
		return (NULL);
	ft_printerr("%p/%p\n", tok_l->next, tok_l->prev);
	return (tok_l);
}

bool	ft_expan_dquote(t_shell *shell, t_list *tok_c, int *status)
{
	t_token	*tok;
	// char	*new_str;
	// char	*var;
	int		i;

	(void) status;
	if (!shell || !tok_c)
		return (false);
	tok = tok_c->content.token;
	i = -1;
	while (tok->value[++i])
	{
		if (tok->value[i] == DOLL && tok->value[i + 1] == Q_MRK)
		{
			if (!ft_expand_status(shell, tok_c, tok->value))
				return (false);
		}
		else if (tok->value[i] == DOLL && (ft_isalpha(tok->value[i + 1])
				|| tok->value[i + 1] == '_'))
			if (!ft_expand_var(shell, tok_c, tok->value, i + 1))
				return (false);
	}
	// free(shell->current_line);
	// shell->current_line = new_str;
	return (true);
}

static void	ft_tok(t_shell *shell, t_list *tok_c, t_list *tok_h, t_list *tok_n)
{
	t_token	*tok;
	bool	head;

	tok = tok_c->content.token;
	head = true;
	if (tok_h == tok_c)
		head = true;
	if (!tok_c->next)
		tok->type = TOKEN_WORD;
	else
	{
		tok_c = ft_getexpan(shell, tok_h, &tok_c, tok_c->next->content.token);
		if (tok_c)
			tok_n = tok_c->next;
	}
	if (head && tok_c)
	{
		shell->token = tok_c;
		head = false;
	}
}

int	ft_expansion(t_shell *shell, t_list *tok_h, int *status)
{
	t_list	*tok_c;
	t_list	*tok_n;
	t_token	*tok;

	if (!shell || !tok_h)
		return (-1);
	tok_c = tok_h;
	while (tok_c)
	{
		ft_printerr("tok: %p\n", tok_c->content.token);
		tok = tok_c->content.token;
		tok_n = tok_c->next;
		ft_printerr("PING ...\n");
		if (tok->type == TOKEN_STATUS)
		{
			ft_printerr("PING ... status\n");
			free(tok->value);
			tok->value = ft_itoa(shell->env->last_exit_code);
		}
		else if (tok->type == TOKEN_VAR)
		{
			ft_printerr("PING ... var\n");
			ft_tok(shell, tok_c, tok_h, tok_n);
		}
		else if (tok->type == TOKEN_DQUOTE)
		{
			ft_printerr("PING ... dquote\n");
			ft_expan_dquote(shell, tok_c, status);
		}
		ft_printerr("PING EXPAN\n");
		if (!tok_c)
			return (-1);
		tok_c = tok_n;
	}
	return (0);
}
