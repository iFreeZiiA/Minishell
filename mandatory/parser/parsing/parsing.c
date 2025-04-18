/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:41:24 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 13:56:29 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

t_ast_node	*ft_get_ast(t_shell *shell, t_list *tok_prev, t_list *tok_curr)
{
	t_list			*tok_next;
	t_token_type	type;

	
}

node_type	ft_getast_type()
{
	
}

t_list	*parse_line(t_shell *shell, t_list *tok_h)
{
	t_list			*tok_curr;
	t_ast_node		*ast_curr;
	t_token_type	type;

	if (!shell || !tok_h->content.token)
		return (NULL);
	tok_curr = tok_h;
	type = tok_curr->next->content.token->type;
	if (type == TOKEN_AND || type == TOKEN_OR)
	{
		ast_curr->data = ft_get_ast(shell, tok_curr->prev, tok_curr);
		ast_curr->type = ft_getast_type();
	}
	return (ast_curr);
}

int	ft_parsing(t_shell *shell)
{
	t_list	*ast;

	ast = parse_line(shell, shell->token);
	if (!ast)
		return (1);
	return (0);
}