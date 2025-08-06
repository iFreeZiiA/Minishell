/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:41:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Crée un nœud de commande simple sans redirections
 * 
 * @param args Tableau d'arguments
 * @return t_ast_node* Nouveau nœud ou NULL si erreur
 */
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
