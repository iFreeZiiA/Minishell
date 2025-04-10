/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:57:37 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/10 13:16:48 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	*ft_get_str(t_list *tok_start, t_token *tok_next)
{
	t_list	*current;
	t_token	*tok;
	char	*str;
	
	current = tok_start;
	str = NULL;
	while (current->content.token->value != tok_next->value)
	{
		tok = (t_token *) current->content.token;
		str = ft_strjoin_f(str, tok->value);
		tok_start = current->next;
		ft_clean_node(current);
		current = tok_start;
	}
	if (!current)
		return (NULL);
	return (str);
}

bool	ft_new_list(t_list *tok_start, t_list *tok_next)
{
	t_list	*first;
	t_token	*new;
	char	*str;

	if (!tok_start || !tok_next)
		return (false);
	first = tok_start->prev;
	str = ft_get_str(tok_start, (t_token *) tok_next->content.token);
	if (!str)
	{
		ft_lstleak(&first, ft_clean_node);
		return (false);
	}
	new = (t_token *) malloc(sizeof(t_token));
	new->type = TOKEN_DQUOTE;
	new->value = str;
	first->next = ft_lstnew((void *) new);
	first->next->next = tok_next;
	return (true);
}

t_list	*ft_get_end(t_list *start, t_token_type type)
{
	
}

int	ft_lexer_checker(t_shell *shell)
{
	t_list	*tok_current;
	t_token	*tok;

	tok_current = shell->token;
	while (tok_current)
	{
		tok = (t_token *) tok_current->content.token;
		if (!tok)
			return (-1);
		if (tok->type == TOKEN_QUOTE || tok->type == TOKEN_DQUOTE)
			ft_new_list(tok_current, ft_get_end(tok_current, tok->type));
		tok_current = tok_current->next;
	}
	return (0);
}