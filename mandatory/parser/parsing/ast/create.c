/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:24:19 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Creates a command structure with arguments and redirections
 * 
 * This helper function allocates and initializes a command structure
 * with the provided arguments array and redirections list.
 * 
 * @param args Array of command arguments
 * @param redirs List of redirections for this command
 * @return t_command* Pointer to created command structure, or NULL on failure
 */
static t_command	*ft_create_command_struct(char **args, t_list *redirs)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = args;
	cmd->redirs = redirs;
	return (cmd);
}

/**
 * @brief Creates an AST node for a command with arguments and redirections
 * 
 * This function creates a command AST node by first creating a command
 * structure and then wrapping it in an AST node.
 * 
 * @param args Array of command arguments
 * @param redirs List of redirections for this command
 * @return t_ast_node* Pointer to created AST node, or NULL on failure
 */
t_ast_node	*ft_new_cmd_node(char **args, t_list *redirs)
{
	t_command	*cmd;
	t_ast_node	*node;

	cmd = ft_create_command_struct(args, redirs);
	if (!cmd)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
	{
		free(cmd);
		return (NULL);
	}
	node->type = NODE_COMMAND;
	node->data = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/**
 * @brief Converts a token type to its corresponding AST node type
 * 
 * This function maps token types for operators to their equivalent
 * AST node types for proper tree construction.
 * 
 * @param token_type Token type to convert
 * @return node_type Corresponding AST node type
 */
node_type	ft_tok_to_node(t_token_type token_type)
{
	if (token_type == TOKEN_PIPE)
		return (NODE_PIPE);
	if (token_type == TOKEN_AND)
		return (NODE_AND);
	if (token_type == TOKEN_OR)
		return (NODE_OR);
	return (NODE_COMMAND);
}

/**
 * @brief Creates an AST node for binary operators (pipe, AND, OR)
 * 
 * This function creates an operator AST node with left and right child nodes.
 * The operator type determines the node type in the AST.
 * 
 * @param type Token type of the operator
 * @param left Left child AST node
 * @param right Right child AST node
 * @return t_ast_node* Pointer to created operator node, or NULL on failure
 */
t_ast_node	*ft_new_op_node(t_token_type type, t_ast_node *left,
	t_ast_node *right)
{
	t_ast_node	*node;
	node_type	ntype;

	ntype = ft_tok_to_node(type);
	if (ntype == NODE_COMMAND)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = ntype;
	node->data = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

/**
 * @brief Creates an AST node for grouped expressions (parentheses)
 * 
 * This function creates a group AST node that wraps content within
 * parentheses, allowing for proper precedence handling.
 * 
 * @param content The AST node representing the content inside parentheses
 * @return t_ast_node* Pointer to created group node, or NULL on failure
 */
t_ast_node	*ft_new_grp_node(t_ast_node *content)
{
	t_ast_node	*node;

	if (!content)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_GROUP;
	node->data = NULL;
	node->left = content;
	node->right = NULL;
	return (node);
}
