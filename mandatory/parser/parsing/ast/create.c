/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:18:55 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

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

t_ast_node	*ft_create_command_node(char **args, t_list *redirs)
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

node_type	ft_token_to_node_type(t_token_type token_type)
{
	if (token_type == TOKEN_PIPE)
		return (NODE_PIPE);
	if (token_type == TOKEN_AND)
		return (NODE_AND);
	if (token_type == TOKEN_OR)
		return (NODE_OR);
	return (NODE_COMMAND);
}

t_ast_node	*ft_create_op_node(t_token_type type, t_ast_node *left,
	t_ast_node *right)
{
	t_ast_node	*node;
	node_type	ntype;

	ntype = ft_token_to_node_type(type);
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

t_ast_node	*ft_create_group_node(t_ast_node *content)
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
