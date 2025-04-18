/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:32 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 15:25:05 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

int	ft_getexpan(t_shell *shell, t_list *tok_h, t_token *tok, t_token *tok_n)
{
	t_list	*tok_l;
	t_token	*new_tok;

	if (!shell || !tok_h || !tok || !tok_n)
		return (-1);
	if (tok->type != TOKEN_WORD)
		return (-1);
	new_tok = malloc(sizeof(t_token));
	new_tok->value = ft_getenv_value(shell, tok->value);
	new_tok->type = TOKEN_WORD;
	tok_l = ft_lstnew_tok(new_tok);
	return (1);
}

int	ft_expansion(t_shell *shell, t_list *tok_h)
{
	t_list	*tok_curr;
	t_token	*tok;

	if (!shell || !tok_h)
		return (-1);
	tok_curr = tok_h;
	while (tok_curr)
	{
		tok = tok_curr->content.token;
		if (tok->type == TOKEN_STATUS)
			tok->value = ft_itoa(shell->env->last_exit_code);
		else if (tok->type == TOKEN_VAR)
		ft_getexpan(shell, tok_h, tok, tok_curr->next->content.token);
		tok_curr = tok_curr->next;
	}
}
