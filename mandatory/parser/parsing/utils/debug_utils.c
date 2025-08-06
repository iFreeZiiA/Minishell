/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:06:43 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

char	*ft_get_token_type_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_OR)
		return ("OR");
	if (type == TOKEN_AND)
		return ("AND");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_PAREN_OPEN)
		return ("LPAREN");
	if (type == TOKEN_PAREN_CLOSE)
		return ("RPAREN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_APPEND)
		return ("APPEND");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

void	ft_debug_tokens(t_list *tokens)
{
	t_list	*current;
	int		count;

	current = tokens;
	count = 1;
	ft_printf("DEBUG: === TOKEN ANALYSIS ===\n");
	while (current)
	{
		ft_printf("DEBUG: Token %d - Type: %s, Value: '%s'\n", 
				count, 
				ft_get_token_type_name(current->content.token->type),
				current->content.token->value ? current->content.token->value : "NULL");
		current = current->next;
		count++;
	}
	ft_printf("DEBUG: === END TOKENS ===\n");
}
