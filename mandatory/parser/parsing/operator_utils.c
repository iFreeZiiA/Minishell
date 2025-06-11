/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:43:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Finds operator token within expression bounds
 * 
 * @param start Expression start
 * @param end Expression end
 * @param target_type Target operator type
 * @return t_list* Operator token or NULL if not found
 */
t_list	*ft_find_operator(t_list *start, t_list *end, t_token_type target_type)
{
	t_list	*curr;
	int		paren_depth;

	curr = start;
	paren_depth = 0;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_PAREN_OPEN)
			paren_depth++;
		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
			paren_depth--;
		else if (paren_depth == 0 && curr->content.token->type == target_type)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Converts token type to AST node type
 * 
 * @param token_type Token type
 * @return node_type Corresponding AST node type
 */
node_type	ft_token_to_node_type(t_token_type token_type)
{
	if (token_type == TOKEN_PIPE)
		return (NODE_PIPE);
	else if (token_type == TOKEN_AND)
		return (NODE_AND);
	else if (token_type == TOKEN_OR)
		return (NODE_OR);
	else
		return (NODE_COMMAND);
}

/**
 * @brief Finds matching closing parenthesis
 * 
 * @param start Opening parenthesis token
 * @return t_list* Closing parenthesis token or NULL if not found
 */
t_list	*ft_find_matching_paren(t_list *start)
{
	t_list	*curr;
	int		depth;

	if (!start || start->content.token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	curr = start->next;
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
 * @brief Checks if token is a logical operator
 * 
 * @param token Token to check
 * @return int 1 if logical operator, 0 otherwise
 */
int	ft_is_logical_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_AND
		|| token->type == TOKEN_OR
		|| token->type == TOKEN_PIPE);
}

/**
 * @brief Gets operator precedence level
 * 
 * @param type Token type
 * @return int Precedence level (higher = more precedent)
 */
int	ft_get_operator_precedence(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (3);
	else if (type == TOKEN_AND)
		return (2);
	else if (type == TOKEN_OR)
		return (1);
	else
		return (0);
}
