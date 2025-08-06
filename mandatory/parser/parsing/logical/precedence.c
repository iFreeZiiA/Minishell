/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precedence.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 18:01:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

#include "../../../../header/minishell.h"

/**
 * @brief Crée une sous-liste de tokens entre start et end (inclus)
 * 
 * @param start Token de début
 * @param end Token de fin
 * @return t_list* Nouvelle liste de tokens copiés
 */
static t_list *ft_create_token_sublist(t_list *start, t_list *end)
{
	t_list *result = NULL;
	t_list *curr = start;
	t_list *new_node;
	t_token *token_copy;
	
	if (!start || !end)
		return (NULL);
		
	while (curr && curr != end->next)
	{
		// Copier le token
		token_copy = malloc(sizeof(t_token));
		if (!token_copy)
		{
			ft_lstfree_t(result);
			return (NULL);
		}
		token_copy->type = curr->content.token->type;
		token_copy->value = ft_strdup(curr->content.token->value);
		if (!token_copy->value)
		{
			free(token_copy);
			ft_lstfree_t(result);
			return (NULL);
		}
		
		// Créer le nœud de liste
		new_node = ft_lstnew_tok(token_copy);
		if (!new_node)
		{
			free(token_copy->value);
			free(token_copy);
			ft_lstfree_t(result);
			return (NULL);
		}
		
		// Ajouter à la liste
		if (!result)
			result = new_node;
		else
		{
			t_list *last = result;
			while (last->next)
				last = last->next;
			last->next = new_node;
			new_node->prev = last;
		}
		curr = curr->next;
	}
	
	return (result);
}

/**
 * @brief Libère une sous-liste de tokens créée par ft_create_token_sublist
 * 
 * @param sublist Sous-liste à libérer
 */
static void ft_free_token_sublist(t_list *sublist)
{
	t_list *curr = sublist;
	t_list *next;
	
	while (curr)
	{
		next = curr->next;
		if (curr->content.token)
		{
			free(curr->content.token->value);
			free(curr->content.token);
		}
		free(curr);
		curr = next;
	}
}

/**
 * @brief Converts token type to AST node type
 * 
 * @param token_type Token type to convert
 * @return node_type Corresponding AST node type
 */

/**
 * @brief Finds operator with specific precedence
 * 
 * @param start Start of token range
 * @param end End of token range
 * @param precedence Target precedence level
 * @return t_list* Operator token or NULL
 */
t_list	*ft_find_operator_by_precedence(t_list *start, t_list *end, int precedence)
{
	t_list	*curr;
	t_token	*token;
	int		current_prec;

	if (!start || !end)
		return (NULL);
	curr = start;
	while (curr && curr != end->next)
	{
		token = curr->content.token;
		if (ft_is_logical_operator_token(token) || token->type == TOKEN_PIPE)
		{
			current_prec = ft_get_operator_precedence(token->type);
			if (current_prec == precedence)
				return (curr);
		}
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Creates operator AST node
 * 
 * @param op_type Operator type
 * @param left Left child
 * @param right Right child
 * @return t_ast_node* New operator node
 */
static t_ast_node	*ft_create_op_node(node_type op_type, t_ast_node *left, t_ast_node *right)
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
 * @brief Parses expression with operator precedence
 * 
 * @param shell Shell structure
 * @param start Start token
 * @param end End token
 * @return t_ast_node* Parsed expression AST
 */
t_ast_node	*ft_parse_logical_expression(t_shell *shell, t_list *start, t_list *end)
{
	t_list		*op_token;
	t_ast_node	*left;
	t_ast_node	*right;
	node_type	op_type;

	// printf("DEBUG: ft_parse_logical_expression called\n");
	
	if (!start || !end)
	{
		// printf("DEBUG: start or end is NULL\n");
		return (NULL);
	}
	
	// printf("DEBUG: Looking for operators with precedence 1, 2, 3\n");
	op_token = ft_find_operator_by_precedence(start, end, 1);
	if (!op_token)
		op_token = ft_find_operator_by_precedence(start, end, 2);
	if (!op_token)
		op_token = ft_find_operator_by_precedence(start, end, 3);
	
	if (!op_token)
	{
		// printf("DEBUG: No operator found, calling ft_parser\n");
		// Créer une sous-liste de tokens pour ft_parser
		t_list *sub_tokens = ft_create_token_sublist(start, end);
		if (!sub_tokens)
			return (NULL);
		t_ast_node *result = ft_parser(sub_tokens, shell);
		ft_free_token_sublist(sub_tokens);
		return (result);
	}
	
	// printf("DEBUG: Operator found, parsing left and right\n");
	left = ft_parse_logical_expression(shell, start, op_token->prev);
	right = ft_parse_logical_expression(shell, op_token->next, end);
	
	// printf("DEBUG: left = %p, right = %p\n", left, right);
	
	if (!left || !right)
	{
		// printf("DEBUG: left or right is NULL\n");
		return (NULL);
	}
	op_type = ft_token_to_node_type(op_token->content.token->type);
	// printf("DEBUG: Creating operator node with type %d\n", op_type);
	return (ft_create_op_node(op_type, left, right));
}
