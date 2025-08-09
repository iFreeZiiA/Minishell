/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precedence_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:50:00 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 16:10:28 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

t_ast_node	*ft_parse_simple_command_precedence(t_shell *shell, t_list *start,
		t_list *end)
{
	t_list		*sub_tokens;
	t_ast_node	*result;

	sub_tokens = ft_create_token_sublist_precedence(start, end);
	if (!sub_tokens)
		return (NULL);
	result = ft_parser(sub_tokens, shell);
	ft_free_token_sublist_precedence(sub_tokens);
	return (result);
}
