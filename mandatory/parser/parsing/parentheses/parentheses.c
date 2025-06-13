/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 10:23:26 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Finds the opening parenthesis for a given position
 * 
 * @param start Starting token
 * @param current Current position
 * @return t_list* Opening parenthesis token
 */
static t_list	*ft_find_opening_paren(t_list *start, t_list *current)
{
	int		depth;
	t_list	*curr;
	t_token	*token;

	depth = 0;
	curr = current;
	while (curr && curr != start->next)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_CLOSE)
			depth++;
		else if (token->type == TOKEN_PAREN_OPEN)
		{
			depth--;
			if (depth == 0)
				return (curr);
		}
		curr = curr->prev;
	}
	return (NULL);
}

/**
 * @brief Parses content between parentheses
 * 
 * @param shell Shell structure
 * @param start Opening parenthesis
 * @param end Closing parenthesis
 * @return t_ast_node* Parsed AST node
 */
static t_ast_node	*ft_parse_paren_content(t_shell *shell, t_list *start,
	t_list *end)
{
	t_list		*content_start;
	t_ast_node	*content_node;

	content_start = start->next;
	if (!content_start || content_start == end)
		return (NULL);
	content_node = ft_parse_expression(shell, shell->token,
			content_start, end);
	return (content_node);
}

/**
 * @brief Creates a group node from parentheses content
 * 
 * @param content Content AST node
 * @return t_ast_node* Group node
 */
static t_ast_node	*ft_create_paren_group(t_ast_node *content)
{
	t_ast_node	*group_node;

	if (!content)
		return (NULL);
	group_node = ft_create_group_node(content);
	return (group_node);
}

/**
 * @brief Handles parentheses in token processing
 * 
 * @param shell Shell structure
 * @param curr Current token
 * @return t_ast_node* Parsed parentheses group
 */
static t_ast_node	*ft_handle_parentheses_token(t_shell *shell, t_list *curr)
{
	t_list		*opening;
	t_list		*closing;
	t_ast_node	*content;
	t_ast_node	*group;
	t_token		*token;

	token = curr->content.token;
	if (token->type == TOKEN_PAREN_OPEN)
	{
		closing = ft_find_matching_paren(curr);
		content = ft_parse_paren_content(shell, curr, closing);
		group = ft_create_paren_group(content);
		return (group);
	}
	else if (token->type == TOKEN_PAREN_CLOSE)
	{
		opening = ft_find_opening_paren(shell->token, curr);
		content = ft_parse_paren_content(shell, opening, curr);
		group = ft_create_paren_group(content);
		return (group);
	}
	return (NULL);
}

/**
 * @brief Main parentheses parsing function
 * 
 * @param shell Shell structure
 * @param token_h Token list head
 * @param start Start of expression
 * @param end End of expression
 * @return t_ast_node* Parsed AST with parentheses handling
 */
t_ast_node	*ft_parentheses(t_shell *shell, t_list *token_h, t_list *start,
	t_list *end)
{
	t_list		*curr;
	t_ast_node	*result;
	t_token		*token;

	if (!shell || !token_h || !start)
		return (NULL);
	curr = start;
	while (curr && curr != end)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_OPEN ||
			token->type == TOKEN_PAREN_CLOSE)
		{
			result = ft_handle_parentheses_token(shell, curr);
			if (result)
				return (result);
		}
		curr = curr->next;
	}
	return (ft_parse_expression(shell, token_h, start, end));
}
