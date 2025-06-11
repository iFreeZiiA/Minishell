/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/11 11:20:18 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Finds the matching closing parenthesis for an opening parenthesis
 * 
 * @param open_paren Token containing the opening parenthesis
 * @return t_list* Token containing the matching closing parenthesis, NULL if not found
 */
t_list	*ft_find_matching_paren(t_list *open_paren)
{
	t_list	*curr;
	int		depth;

	if (!open_paren || open_paren->content.token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	
	curr = open_paren->next;
	depth = 1;
	
	while (curr && depth > 0)
	{
		if (curr->content.token->type == TOKEN_PAREN_OPEN)
			depth++;
		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
			depth--;
		
		if (depth == 0)
			return (curr);
		
		curr = curr->next;
	}
	
	return (NULL);
}

/**
 * @brief Validates parentheses in the token list
 * 
 * @param token_h Head of the token list
 * @return bool true if parentheses are valid, false otherwise
 */
bool	ft_validate_parentheses(t_list *token_h)
{
	t_list	*curr;
	int		depth;

	if (!token_h)
		return (true);
	
	curr = token_h;
	depth = 0;
	
	while (curr)
	{
		if (curr->content.token->type == TOKEN_PAREN_OPEN)
		{
			depth++;
		}
		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
		{
			depth--;
			if (depth < 0)
				return (false);  // Closing paren without opening
		}
		curr = curr->next;
	}
	
	return (depth == 0);  // Must be balanced
}

/**
 * @brief Parses a group (content within parentheses)
 * 
 * @param shell Shell structure
 * @param open_paren Opening parenthesis token
 * @param close_paren Closing parenthesis token
 * @return t_ast_node* AST node representing the group
 */
t_ast_node	*ft_parse_group(t_shell *shell, t_list *open_paren, t_list *close_paren)
{
	t_ast_node	*content;
	t_ast_node	*group_node;

	if (!shell || !open_paren || !close_paren)
		return (NULL);
	
	if (open_paren->content.token->type != TOKEN_PAREN_OPEN ||
		close_paren->content.token->type != TOKEN_PAREN_CLOSE)
		return (NULL);
	
	// Parse the content between parentheses
	if (open_paren->next == close_paren)
	{
		// Empty parentheses - create empty group
		return (ft_create_group_node(NULL));
	}
	
	// Recursively parse the expression inside the parentheses
	content = ft_parse_expression(shell, shell->token, open_paren->next, close_paren);
	if (!content)
		return (NULL);
	
	// Create group node
	group_node = ft_create_group_node(content);
	if (!group_node)
	{
		ft_free_ast(content);
		return (NULL);
	}
	
	return (group_node);
}

/**
 * @brief Checks if parentheses content is empty (only spaces)
 * 
 * @param open_paren Opening parenthesis
 * @param close_paren Closing parenthesis
 * @return bool true if empty, false otherwise
 */
bool	ft_is_empty_group(t_list *open_paren, t_list *close_paren)
{
	t_list	*curr;

	if (!open_paren || !close_paren)
		return (true);
	
	curr = open_paren->next;
	while (curr && curr != close_paren)
	{
		if (curr->content.token->type != TOKEN_SPACE)
			return (false);
		curr = curr->next;
	}
	
	return (true);
}