/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:14:34 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	ft_print_indent(int level)
{
	int	i;

	i = 0;
	while (i < level)
	{
		ft_printf("  ");
		i++;
	}
}

static char	*ft_get_node_type_str(node_type type)
{
	if (type == NODE_COMMAND)
		return ("COMMAND");
	if (type == NODE_PIPE)
		return ("PIPE");
	if (type == NODE_AND)
		return ("AND");
	if (type == NODE_OR)
		return ("OR");
	if (type == NODE_GROUP)
		return ("GROUP");
	return ("UNKNOWN");
}

static void	ft_print_command_args(t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		ft_printf("\"%s\"", cmd->args[i]);
		if (cmd->args[i + 1])
			ft_printf(" ");
		i++;
	}
}

static void	ft_print_node_content(t_ast_node *ast, int level)
{
	t_command	*cmd;

	if (ast->type == NODE_COMMAND)
	{
		ft_printf("\n");
		ft_print_indent(level + 1);
		ft_printf("Args: ");
		cmd = (t_command *)ast->data;
		ft_print_command_args(cmd);
		ft_printf("\n");
	}
	else if (ast->type == NODE_GROUP)
	{
		ft_printf(" (subshell)\n");
		if (ast->left)
			ft_print_ast_tree(ast->left, level + 1);
	}
	else
	{
		ft_printf("\n");
		if (ast->left)
			ft_print_ast_tree(ast->left, level + 1);
		if (ast->right)
			ft_print_ast_tree(ast->right, level + 1);
	}
}

void	ft_print_ast_tree(t_ast_node *ast, int level)
{
	if (!ast)
	{
		ft_print_indent(level);
		ft_printf("(null)\n");
		return ;
	}
	ft_print_indent(level);
	ft_printf("%s", ft_get_node_type_str(ast->type));
	ft_print_node_content(ast, level);
}
