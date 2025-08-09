/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_logic.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:33:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Trouve l'opérateur de plus faible précédence en dehors des parenthèses
 * @param start Début de la recherche
 * @param end Fin de la recherche
 * @return t_list* Token de l'opérateur trouvé ou NULL
 */
static t_list	*ft_find_operator_outside_parens(t_list *start, t_list *end)
{
	t_list	*current;
	t_list	*result;
	t_token	*token;
	int		paren_depth;
	int		min_precedence;
	int		current_prec;

	result = NULL;
	min_precedence = 999;
	paren_depth = 0;
	current = start;
	while (current && current != end->next)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (token->type == TOKEN_PAREN_OPEN)
				paren_depth++;
			else if (token->type == TOKEN_PAREN_CLOSE)
				paren_depth--;
			else if (paren_depth == 0 && (ft_is_logical_operator_token(token)
					|| token->type == TOKEN_PIPE))
			{
				current_prec = ft_get_operator_precedence(token->type);
				if (current_prec <= min_precedence)
				{
					min_precedence = current_prec;
					result = current;
				}
			}
		}
		current = current->next;
	}
	return (result);
}

/**
 * @brief Parse une expression logique en tenant compte des parenthèses
 * @param shell Structure shell
 * @param start Début de la liste de tokens
 * @param end Fin de la liste de tokens
 * @return t_ast_node* Nœud AST de l'expression
 */
t_ast_node	*ft_parse_logical_with_parens(t_shell *shell, t_list *start,
		t_list *end)
{
	t_list		*op_token;
	t_list		*open_paren;
	t_list		*close_paren;
	t_ast_node	*left;
	t_ast_node	*right;

	if (!shell || !start || !end)
		return (NULL);
	open_paren = ft_find_first_open_paren(start);
	if (open_paren && open_paren == start)
	{
		close_paren = ft_find_matching_paren(open_paren);
		if (close_paren && close_paren == end)
			return (ft_parse_group(shell, open_paren, close_paren));
	}
	op_token = ft_find_operator_outside_parens(start, end);
	if (!op_token)
		return (ft_parse_simple_command_precedence(shell, start, end));
	left = ft_parse_logical_with_parens(shell, start, op_token->prev);
	right = ft_parse_logical_with_parens(shell, op_token->next, end);
	if (!left || !right)
		return (NULL);
	return (ft_create_operator_node(ft_tok_to_node(op_token->content.token->type),
			left, right));
}

/**
 * @brief Traite les parenthèses dans une expression et retourne l'AST approprié
 * @param shell Structure shell
 * @param tokens Liste de tokens
 * @return t_ast_node* Nœud AST de l'expression traitée
 */
t_ast_node	*ft_handle_parentheses_in_expression(t_shell *shell, t_list *tokens)
{
	if (!shell || !tokens)
		return (NULL);
	if (ft_validate_parentheses_balance(tokens) != 0)
		return (NULL);
	if (!ft_has_parentheses(tokens))
		return (ft_parse_logical_expression(shell, tokens, ft_lstlast(tokens)));
	return (ft_parse_logical_with_parens(shell, tokens, ft_lstlast(tokens)));
}
