/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_tok.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:42:20 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/12 15:45:16 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	*ft_tokerr(t_list *tok_n, t_token *tok)
{
	if (tok_n)
		ft_clean_node_tok(tok_n);
	if (tok)
	{
		if (tok->value)
			free(tok->value);
		free(tok);
		tok = NULL;
	}
	return (NULL);
}

t_token	*ft_set_token(t_shell *shell, t_list **tok_c)
{
	t_token	*new_token;
	char	*env_value;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	env_value = ft_getenv_value(shell, (*tok_c)->next->content.token->value, 0,
			ft_strlen((*tok_c)->next->content.token->value));
	if (!env_value)
		return (ft_tokerr(NULL, new_token));
	new_token->value = env_value;
	new_token->type = TOKEN_WORD;
	return (new_token);
}

/**
 * @brief Expands a variable token into its value
 * 
 * @param shell The shell structure
 * @param tok_h The token list head
 * @param tok_c Current token node
 * @param tok_n Next token node
 * @return t_list* Updated current token
 */
t_list	*ft_expand_token(t_shell *shell, t_list **tok_h,
		t_list **tok_c, t_list **tok_n)
{
	t_token	*token;
	t_token	*new_token;
	t_list	*new_node;

	if (!shell || !(*tok_h) || !(*tok_c) || !(*tok_c)->content.token)
		return (NULL);
	token = (*tok_c)->content.token;
	if (!(*tok_c)->next || (*tok_c)->next->content.token->type != TOKEN_WORD)
	{
		token->type = TOKEN_WORD;
		return (*tok_c);
	}
	new_token = ft_set_token(shell, tok_c);
	if (!new_token)
		return (NULL);
	new_node = ft_lstnew_tok(new_token);
	if (!new_node)
		return (ft_tokerr(NULL, new_token));
	if (!ft_lstreplace_n(tok_c, new_node, ft_clean_node_tok, 2))
		return (ft_tokerr(new_node, NULL));
	if (!(*tok_c)->prev)
		*tok_h = *tok_c;
	*tok_n = (*tok_c)->next;
	return (*tok_c);
}
