/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/27 15:46:18 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Finds matching closing parenthesis from opening one
 * 
 * @param start Token list starting from opening parenthesis
 * @return t_list* Matching closing parenthesis token or NULL
 */
t_list	*ft_find_matching_paren(t_list *start)
{
	t_list	*current;
	int		depth;
	t_token	*token;

	if (!start || !start->content.token)
		return (NULL);
	if (start->content.token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	current = start->next;
	depth = 1;
	while (current && depth > 0)
	{
		token = current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			depth++;
		else if (token->type == TOKEN_PAREN_CLOSE)
			depth--;
		if (depth == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
<<<<<<< HEAD
 * @brief Validates parentheses balance in token list
 * 
 * @param token_h Token list head
 * @return int Depth balance (0 = balanced, >0 = missing close, <0 = extra close)
=======
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
>>>>>>> origin/dev
 */
int	ft_validate_parentheses_balance(t_list *token_h)
{
	t_list	*current;
	int		balance;
	t_token	*token;

	if (!token_h)
		return (0);
	current = token_h;
	balance = 0;
	while (current)
	{
		token = current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			balance++;
		else if (token->type == TOKEN_PAREN_CLOSE)
			balance--;
		current = current->next;
	}
	return (balance);
}

/**
<<<<<<< HEAD
 * @brief Validates parentheses content between start and end
 * 
 * @param start Opening parenthesis position
 * @param end Closing parenthesis position
 * @return bool true if content is valid
=======
 * @brief Creates a group AST node from parenthesized content
 * 
 * This function wraps parsed content in a group node, which represents
 * a subshell or grouped expression that should be treated as a single unit.
 * 
 * @param content The parsed AST content to wrap in a group
 * @return t_ast_node* Group AST node containing the content
>>>>>>> origin/dev
 */
bool	ft_validate_parentheses_content(t_list *start, t_list *end)
{
	t_list	*current;
	bool	has_content;

	if (!start || !end)
		return (false);
	if (start->next == end)
		return (false);
	current = start->next;
	has_content = false;
	while (current && current != end)
	{
		if (current->content.token->type == TOKEN_WORD)
			has_content = true;
		current = current->next;
	}
	return (has_content);
}

/**
 * @brief Parses tokens inside parentheses group
 * 
 * @param shell Shell structure
 * @param start Opening parenthesis token
 * @param end Closing parenthesis token
 * @return t_ast_node* Group AST node with NODE_GROUP type
 */
t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end)
{
	t_ast_node	*group_node;
	t_ast_node	*content_node;
	t_list		*content_start;

	if (!shell || !start || !end)
		return (NULL);
	if (!ft_validate_parentheses_content(start, end))
		return (NULL);
	content_start = start->next;
	content_node = ft_parser(content_start, shell);
	if (!content_node)
		return (NULL);
	group_node = malloc(sizeof(t_ast_node));
	if (!group_node)
		return (NULL);
	group_node->type = NODE_GROUP;
	group_node->data = content_node;
	group_node->left = NULL;
	group_node->right = NULL;
	return (group_node);
}

/**
<<<<<<< HEAD
 * @brief Main entry point for parentheses parsing
 * 
 * @param shell Shell structure
 * @param token_h Token list head
 * @param start Opening parenthesis token
 * @param end Closing parenthesis token
 * @return t_ast_node* Parsed group node or NULL on error
=======
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
>>>>>>> origin/dev
 */
t_ast_node	*ft_parentheses(t_shell *shell, t_list *token_h, t_list *start,
				t_list *end)
{
	int	balance;

	if (!shell || !token_h)
		return (NULL);
	balance = ft_validate_parentheses_balance(token_h);
	if (balance != 0)
		return (NULL);
	if (start && end)
		return (ft_parse_group(shell, start, end));
	return (NULL);
}
