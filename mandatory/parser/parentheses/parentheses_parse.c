/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:32:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Extrait les tokens entre deux positions (exclusif)
 * @param start Token de début (exclu)
 * @param end Token de fin (exclu)
 * @return t_list* Nouvelle liste contenant les tokens entre start et end
 */
static t_list	*ft_extract_tokens_between(t_list *start, t_list *end)
{
	t_list		*result;
	t_list		*current;
	t_content	content;

	if (!start || !end || start == end)
		return (NULL);
	result = NULL;
	current = start->next;
	while (current && current != end)
	{
		if (current->type == TYPE_TOKEN)
		{
			content.token = ft_new_token(ft_strdup(current->content.token->value),
					current->content.token->type);
			if (!content.token)
			{
				ft_lstfree_t(result);
				return (NULL);
			}
			ft_lstadd_back(&result, content, TYPE_TOKEN);
		}
		current = current->next;
	}
	return (result);
}

/**
 * @brief Parse le contenu d'un groupe de parenthèses
 * @param shell Structure shell
 * @param start Token de parenthèse ouvrante
 * @param end Token de parenthèse fermante
 * @return t_ast_node* Nœud AST du groupe parsé
 */
t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end)
{
	t_list		*inner_tokens;
	t_ast_node	*result;
	t_ast_node	*group_node;

	if (!shell || !start || !end)
		return (NULL);
	if (start->content.token->type != TOKEN_PAREN_OPEN
		|| end->content.token->type != TOKEN_PAREN_CLOSE)
		return (NULL);
	inner_tokens = ft_extract_tokens_between(start, end);
	if (!inner_tokens)
		return (NULL);
	result = ft_enhanced_parser(inner_tokens, shell);
	ft_lstfree_t(inner_tokens);
	if (!result)
		return (NULL);
	group_node = malloc(sizeof(t_ast_node));
	if (!group_node)
	{
		ft_free_ast(result);
		return (NULL);
	}
	group_node->type = NODE_GROUP;
	group_node->data = NULL;
	group_node->left = result;
	group_node->right = NULL;
	return (group_node);
}

/**
 * @brief Parse une expression contenant des parenthèses
 * @param shell Structure shell
 * @param tokens Liste de tokens
 * @return t_ast_node* Nœud AST de l'expression avec parenthèses
 */
t_ast_node	*ft_parse_parentheses_expression(t_shell *shell, t_list *tokens)
{
	t_list	*open_paren;
	t_list	*close_paren;

	if (!shell || !tokens)
		return (NULL);
	if (ft_validate_parentheses_balance(tokens) != 0)
		return (NULL);
	open_paren = ft_find_first_open_paren(tokens);
	if (!open_paren)
		return (ft_enhanced_parser(tokens, shell));
	close_paren = ft_find_matching_paren(open_paren);
	if (!close_paren)
		return (NULL);
	return (ft_parse_group(shell, open_paren, close_paren));
}
