/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Validates that group tokens are properly bounded by parentheses
 * 
 * This function checks that the start and end tokens of a group are
 * valid opening and closing parentheses respectively.
 * 
 * @param start Starting token (should be opening parenthesis)
 * @param end Ending token (should be closing parenthesis)
 * @return bool true if bounds are valid, false otherwise
 */
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

/**
 * @brief Validates that a group contains non-empty content
 * 
 * This function ensures that there is actual content between the
 * opening and closing parentheses of a group.
 * 
 * @param start Starting token (opening parenthesis)
 * @param end Ending token (closing parenthesis)
 * @return bool true if group has content, false if empty
 */
static bool	ft_validate_group_content(t_list *start, t_list *end)
{
	t_list	*content_start;

	content_start = start->next;
	if (!content_start || content_start == end)
		return (false);
	return (true);
}

/**
 * @brief Parses the content inside parentheses of a group
 * 
 * This function recursively parses the expression contained within
 * the parentheses by calling the main expression parser.
 * 
 * @param shell Shell structure for parsing context
 * @param start Starting token (opening parenthesis)
 * @param end Ending token (closing parenthesis)
 * @return t_ast_node* AST node representing the parsed content
 */
static t_ast_node	*ft_parse_group_content(t_shell *shell, t_list *start,
			t_list *end)
{
	t_list		*content_start;
	t_ast_node	*content;

	content_start = start->next;
	content = ft_parse_expression(shell, shell->token, content_start, end);
	return (content);
}

/**
 * @brief Parses a complete parenthesized group expression
 * 
 * This function handles the complete process of parsing a group:
 * 1. Validates that the group is properly bounded by parentheses
 * 2. Ensures the group contains valid content
 * 3. Recursively parses the content inside the parentheses
 * 4. Creates a group AST node wrapping the parsed content
 * 
 * @param shell Shell structure for parsing context
 * @param start Starting token (opening parenthesis)
 * @param end Ending token (closing parenthesis)
 * @return t_ast_node* Group AST node, or NULL on failure
 */
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
