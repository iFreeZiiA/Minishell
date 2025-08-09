/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:34:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:36:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Crée un nœud d'opérateur avec deux enfants
 * @param op_type Type d'opérateur
 * @param left Nœud enfant gauche
 * @param right Nœud enfant droit
 * @return t_ast_node* Nouveau nœud d'opérateur
 */
t_ast_node	*ft_create_operator_node(t_node_type op_type, t_ast_node *left,
		t_ast_node *right)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = op_type;
	node->data = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

/**
 * @brief Point d'entrée principal pour le parsing avec parenthèses
 * @param shell Structure shell
 * @param tokens Liste de tokens à parser
 * @return t_ast_node* Nœud AST résultant
 */
t_ast_node	*ft_parse_with_parentheses(t_shell *shell, t_list *tokens)
{
	if (!shell || !tokens)
		return (NULL);
	if (ft_validate_parentheses_balance(tokens) != 0)
	{
		write(STDERR_FILENO, "minishell: syntax error: unmatched parentheses\n",
			47);
		return (NULL);
	}
	return (ft_handle_parentheses_in_expression(shell, tokens));
}
