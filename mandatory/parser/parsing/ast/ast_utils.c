/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:07:47 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

t_ast_node	*ft_create_simple_command_node(char **args)
{
	t_ast_node	*node;
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
	{
		free(cmd);
		return (NULL);
	}
	cmd->args = args;
	cmd->redirs = NULL;
	node->type = NODE_COMMAND;
	node->data = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static void	ft_free_command_data(t_command *cmd)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	free(cmd);
}

void	ft_free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->data)
		ft_free_command_data((t_command *)node->data);
	ft_free_ast_node(node->left);
	ft_free_ast_node(node->right);
	free(node);
}
