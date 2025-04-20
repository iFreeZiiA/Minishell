/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:32 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/20 15:04:03 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

t_list	*ft_getexpan(t_shell *shell, t_list *tok_h, t_list **tok_c, t_token *tok_n)
{
	t_list	*tok_l;
	t_token	*new_tok;

	if (!shell || !tok_h || !(*tok_c)->content.token || !tok_n)
		return (NULL);
	ft_printerr("PING\n");
	if (!(*tok_c)->next || (*tok_c)->next->content.token->type != TOKEN_WORD)
	{
		(*tok_c)->content.token->type = TOKEN_WORD;
		return (*tok_c);
	}
	new_tok = malloc(sizeof(t_token));
	if (!new_tok)
		return (NULL);
	ft_printerr("Ping getexpan 0\n");
	new_tok->value = ft_getenv_value(shell, (*tok_c)->content.token->value);
	ft_printerr("Ping getexpan 1\n");
	new_tok->type = TOKEN_WORD;
	tok_l = ft_lstnew_tok(new_tok);
	ft_printerr("Ping getexpan 2\n");
	if (!ft_lstreplace_n(tok_c, tok_l, ft_clean_node_tok, 2))
		return (NULL);
	ft_printerr("Ping getexpan 3\n");
	return (tok_l);
}

// char	*ft_expand_var(char *str, int i, bool var)
// {
// 	char	*new_str;
// 	int		j;

// 	if (!str)
// 		return (NULL);
// 	if (i < 0)
// 	{
// 		free(str);
// 		return (NULL);
// 	}

// }

// int	ft_expan_dquote(t_token *tok)
// {
// 	char	*str;
// 	int		i;

// 	if (!tok)
// 		return (-1);
// 	str = tok->value;
// 	i = -1;
// 	while (str[++i])
// 	{
// 		if (str[i] == '$')
// 		{
// 			if (str[i] == '$' && str [i + 1] == '?')
// 				str = ft_expand_var(str, i, false);
// 			else
// 				str = ft_expand_var(str, i, true);
// 			if (!str)
// 				return (-1);
// 		}
// 	}
// }

int	ft_expansion(t_shell *shell, t_list *tok_h)
{
	t_list	*tok_c;
	t_list	*tok_n;
	t_token	*tok;

	if (!shell || !tok_h)
		return (-1);
	tok_c = tok_h;
	while (tok_c)
	{
		tok = tok_c->content.token;
		tok_n = tok_c->next;
		ft_printerr("Ping ft_expansion 0: %s, %d\n", tok->value, tok->type);
		if (tok->type == TOKEN_STATUS)
		{
			ft_printerr("Ping ft_expansion 1: %s, %d\n", tok->value, tok->type);
			tok->value = ft_itoa(shell->env->last_exit_code);
		}
		else if (tok->type == TOKEN_VAR)
		{
			ft_printerr("Ping ft_expansion 2: %s, %d\n", tok->value, tok->type);
			if (!tok_c->next)
				tok->type = TOKEN_WORD;
			else
			{
				tok_c = ft_getexpan(shell, tok_h, &tok_c, tok_c->next->content.token);
				if (tok_c)
					tok_n = tok_c->next;
			}
		}
		// else if (tok->type == TOKEN_DQUOTE)
		// {
		// 	ft_printerr("Ping ft_expansion 3: %s, %d\n", tok->value, tok->type);
		// 	ret = ft_expan_dquote(tok);
		// }
		if (!tok_c)
		{
			ft_printerr("Ping ft_expansion 5\n");
			return (-1);
		}
		ft_printerr("Ping ft_expansion 6\n");
		tok_c = tok_n;
	}
	ft_printerr("Ping ft_expansion 7: %s\n", tok_c->content.token->value);
	return (0);
}
