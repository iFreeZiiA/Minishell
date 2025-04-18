/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:32 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/14 17:45:32 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	ft_getenv_value(t_shell *shell, t_list *tok_h, t_token *tok, t_token *tok_n)
{
	if (!shell || !tok_h || !tok || !tok_n)
		return (-1);
	if ()
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
			tok->value = shell->env->last_exit_code;
		else if (tok->type == TOKEN_VAR)
			ft_getenv_value(shell, tok_h, tok, tok_curr->next->content.token);
		tok_curr = tok_curr->next;
	}
}
