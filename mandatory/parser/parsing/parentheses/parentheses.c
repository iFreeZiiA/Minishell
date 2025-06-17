/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Finds the opening parenthesis that matches a closing parenthesis
 * 
 * This function traverses backwards from a closing parenthesis to find
 * its corresponding opening parenthesis, tracking nesting depth to
 * handle nested parentheses correctly.
 * 
 * @param start Starting boundary for the search
 * @param current Position of the closing parenthesis
 * @return t_list* Pointer to matching opening parenthesis token, or NULL
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
 * @brief Parses the expression content between matching parentheses
 * 
 * This function extracts and parses the expression contained within
 * a pair of parentheses by calling the main expression parser on
 * the content between the opening and closing parentheses.
 * 
 * @param shell Shell structure containing parsing context
 * @param start Opening parenthesis token
 * @param end Closing parenthesis token
 * @return t_ast_node* AST node representing the parsed content
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
 * @brief Creates a group AST node from parenthesized content
 * 
 * This function wraps parsed content in a group node, which represents
 * a subshell or grouped expression that should be treated as a single unit.
 * 
 * @param content The parsed AST content to wrap in a group
 * @return t_ast_node* Group AST node containing the content
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
 * @brief Handles parentheses tokens during parsing
 * 
 * This function processes parentheses tokens by:
 * 1. Finding the matching parenthesis (opening or closing)
 * 2. Parsing the content between the parentheses
 * 3. Creating a group node to represent the parenthesized expression
 * 
 * @param shell Shell structure containing parsing context
 * @param curr Current token (should be a parenthesis)
 * @return t_ast_node* Group AST node for the parenthesized expression
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
 * @brief Main parentheses parsing function for expression ranges
 * 
 * This function processes parentheses within an expression range:
 * 1. Scans through tokens looking for parentheses
 * 2. When found, handles the parenthesized group appropriately
 * 3. Falls back to regular expression parsing if no parentheses found
 * 
 * This enables proper grouping and precedence handling in complex expressions.
 * 
 * @param shell Shell structure containing parsing context
 * @param token_h Head of the complete token list
 * @param start Starting position for parsing
 * @param end Ending position for parsing (exclusive)
 * @return t_ast_node* AST node representing the parsed expression
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
