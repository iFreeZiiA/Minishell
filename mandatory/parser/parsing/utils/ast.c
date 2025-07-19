/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 15:35:58 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	ft_free_command_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	ft_free_command_struct(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		ft_free_command_args(cmd->args);
	if (cmd->redirs)
		ft_lstleak(&cmd->redirs, ft_clean_node_tok);
	free(cmd);
}

void	ft_free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->data)
		ft_free_command_struct((t_command *)node->data);
	if (node->left)
		ft_free_ast_node(node->left);
	if (node->right)
		ft_free_ast_node(node->right);
	free(node);
}

void	ft_print_ast_simple(t_ast_node *ast, int depth)
{
	t_command	*cmd;
	int			i;
	int			j;

	if (!ast)
		return ;
	i = 0;
	while (i < depth)
	{
		ft_printf("  ");
		i++;
	}
	if (ast->type == NODE_COMMAND)
	{
		cmd = (t_command *)ast->data;
		ft_printf("COMMAND: ");
		if (cmd && cmd->args)
		{
			j = 0;
			while (cmd->args[j])
			{
				ft_printf("%s", cmd->args[j]);
				if (cmd->args[j + 1])
					ft_printf(" ");
				j++;
			}
		}
		ft_printf("\n");
	}
}

int	ft_validate_ast_structure(t_ast_node *ast)
{
	t_command	*cmd;

	if (!ast)
		return (-1);
	if (ast->type != NODE_COMMAND)
		return (-1);
	if (!ast->data)
		return (-1);
	cmd = (t_command *)ast->data;
	if (!cmd->args)
		return (-1);
	if (!cmd->args[0])
		return (-1);
	return (0);
}
