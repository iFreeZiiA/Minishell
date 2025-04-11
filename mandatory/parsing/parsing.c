/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:41:24 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/11 19:45:54 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_list	*parse_line(t_shell *shell, t_list *tok_h)
{
	t_list			*tok_curr;
	t_list			*ast_curr;
	t_token_type	type;

	if (!shell || !tok_h->content.token)
		return (NULL);
	tok_curr = tok_curr;
	type = tok_curr->next->content.token->type;
	if (type == TOKEN_AND || type == TOKEN_OR)
		
}

int	ft_parsing(t_shell *shell)
{
	t_list	*ast;

	ast = parse_line(shell, shell->token);
	if (ast)
		return (ast);
	return (0);
}