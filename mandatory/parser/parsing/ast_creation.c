/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:38:42 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Creates a new AST node
 * 
 * @param type Node type
 * @param data Associated data
 * @param left Left child
 * @param right Right child
 * @return t_ast_node* New node or NULL on error
 */
t_ast_node	*ft_create_ast_node(node_type type, void *data,
		t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->data = data;
	node->left = left;
	node->right = right;
	return (node);
}

/**
 * @brief Creates a command AST node
 * 
 * @param cmd Command structure
 * @return t_ast_node* New command node or NULL on error
 */
t_ast_node	*ft_create_command_ast(t_command *cmd)
{
	return (ft_create_ast_node(NODE_COMMAND, cmd, NULL, NULL));
}

/**
 * @brief Creates an operator AST node
 * 
 * @param op_type Operator type
 * @param left Left operand
 * @param right Right operand
 * @return t_ast_node* New operator node or NULL on error
 */
t_ast_node	*ft_create_operator_ast(node_type op_type,
		t_ast_node *left, t_ast_node *right)
{
	return (ft_create_ast_node(op_type, NULL, left, right));
}

/**
 * @brief Creates a group AST node
 * 
 * @param content Group content
 * @return t_ast_node* New group node or NULL on error
 */
t_ast_node	*ft_create_group_ast(t_ast_node *content)
{
	return (ft_create_ast_node(NODE_GROUP, NULL, content, NULL));
}

/**
 * @brief Cleans up failed AST creation
 * 
 * @param left Left node to free
 * @param right Right node to free
 */
void	ft_cleanup_ast_creation(t_ast_node *left, t_ast_node *right)
{
	if (left)
		ft_free_ast(left);
	if (right)
		ft_free_ast(right);
}
