/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_node_tok.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:17:55 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 17:49:54 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib_minishell.h"

void	*ft_clean_node_tok(t_list *tok_curr)
{
	t_token	*tok;

	if (!tok_curr && (tok_curr->type != TYPE_TOKEN))
		return (NULL);
	tok = tok_curr->content.token;
	// tok = (t_token *) tok_curr->content;
	if (tok && tok->value)
		free(tok->value);
	if (tok)
		free(tok);
	free(tok_curr);
	tok_curr = NULL;
	return (NULL);
}
