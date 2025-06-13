/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:12:46 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static bool	ft_validate_group_bounds(t_list *start, t_list *end)
{
	if (!start || !end)
		return (false);
	if (start->content.token->type != TOKEN_PAREN_OPEN)
		return (false);
	if (end->content.token->type != TOKEN_PAREN_CLOSE)
		return (false);
	return (true);
}

static bool	ft_validate_group_content(t_list *start, t_list *end)
{
	t_list	*content_start;

	content_start = start->next;
	if (!content_start || content_start == end)
		return (false);
	return (true);
}

static t_ast_node	*ft_parse_group_content(t_shell *shell, t_list *start,
			t_list *end)
{
	t_list		*content_start;
	t_ast_node	*content;

	content_start = start->next;
	content = ft_parse_expression(shell, shell->token, content_start, end);
	return (content);
}

t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end)
{
	t_ast_node	*content;

	if (!ft_validate_group_bounds(start, end))
		return (NULL);
	if (!ft_validate_group_content(start, end))
		return (NULL);
	content = ft_parse_group_content(shell, start, end);
	if (!content)
		return (NULL);
	return (ft_create_group_node(content));
}
