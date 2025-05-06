/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:32 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 13:05:31 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

t_list	*ft_getexpan(t_shell *shell, t_list *tok_h, t_list **tok_c, t_list **tok_n)
{
	// t_list	*tok_l;`
	t_token	*new_tok;
	char	*str;
	// char	*str_n;
	// int		eov;

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
	// ft_printerr("GETEXPAN: '%s': %d\n", str,  ft_strlen(str));
	ft_expand_var(shell, *tok_c, str, 0);
	// new_tok->value = ft_getenv_value(shell, str, 0, ft_strlen(eov));
	
	// ft_printerr("PING GETEXPAN 1\n");
	// ft_printerr("PING GETEXPAN 2\n");
	// ft_printerr("%p, '%s', %d\n", new_tok, new_tok->value, new_tok->type);
	// tok_l = ft_lstnew_tok(new_tok);
	// tok_l->content.token->type = TOKEN_WORD;
	
	// ft_printerr("PING GETEXPAN 3\n");
	// ft_printerr("%p, '%s', %d\n", tok_l->content.token, tok_l->content.token->value, tok_l->content.token->type);
	// if (!tok_l)
	// 	return (NULL);
	// ft_printerr("PING GETEXPAN 4\n");
	// ft_printerr("tok: %s\n", (*tok_c)->content.token->value);
	// ft_printerr("%d\n", (*tok_n)->type);
	// if (!ft_lstreplace_n(tok_c, tok_l, ft_clean_node_tok, 2))
	// 	return (NULL);
	// ft_printerr("tok_c: %d\n", (*tok_c)->content.token->type);
	// ft_printerr("tok_n: %d\n", (*tok_c)->next->content.token->type);
	*tok_n = (*tok_c)->next;
	// ft_printerr("%p/%p\n", *tok_c, tok_l);
	return (*tok_c);
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

static void	ft_tok(t_shell *shell, t_list **tok_c, t_list *tok_h, t_list **tok_n)
{
	t_token	*tok;
	bool	head;

	tok = (*tok_c)->content.token;
	head = false;
	if (tok_h == *tok_c)
		head = true;
	if (!(*tok_c)->next)
		tok->type = TOKEN_WORD;
	else
	{
		*tok_c = ft_getexpan(shell, tok_h, tok_c, tok_n);
		// ft_printerr("ft_tokkkk: %d\n", (*tok_c)->content.token->type);
		if (*tok_c)
			(*tok_c)->next = *tok_n;
		// ft_printerr("ft_tok: %p\n", (*tok_c)->prev);
	}
	if (head && *tok_c)
	{
		shell->token = *tok_c;
		head = false;
	}
}

int	ft_expansion(t_shell *shell, t_list *tok_h, int *status)
{
	t_list	*tok_c;
	t_list	*tok_n;
	t_token	*tok;

	if (!shell || !tok_h)
	// {
	// 	ft_printerr("ERROR, WHYYYYY\n");
		return (-1);
	// }
	tok_c = tok_h;
	while (tok_c)
	{
		// ft_printerr("tok EXPANSION!!!!!!!!!: %p\n", tok_c->content.token->value);
		tok = tok_c->content.token;
		tok_n = tok_c->next;
		// ft_printerr("PING ...\n");
		if (tok->type == TOKEN_STATUS)
		{
			// ft_printerr("PING ... status\n");
			free(tok->value);
			tok->value = ft_itoa(shell->env->last_exit_code);
		}
		else if (tok->type == TOKEN_VAR)
		// {
		// 	ft_printerr("PING ... var\n");
			ft_tok(shell, &tok_c, tok_h, &tok_n);
			// ft_printerr("expansion: %s\n", tok_c->next->content.token->value);
		// }
		else if (tok->type == TOKEN_DQUOTE)
		// {
		// 	ft_printerr("PING ... dquote\n");
			ft_expan_dquote(shell, tok_c, status);
		// }
		// ft_printerr("tok EXPANSION!!!!!!!!!: %d\n", tok_c->content.token->type);
		if (!tok_c)
		// {
			// ft_printerr("ERROR, WHYYYYY\n");
			return (-1);
		// }
		tok_c = tok_n;
	}
	// ft_printerr("pointer: %p\n", shell->token->next);
	return (0);
}
