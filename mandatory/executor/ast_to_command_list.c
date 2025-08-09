/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_to_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:15:34 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 10:30:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static void	add_command_node(t_list **cmds, t_command *cmd)
{
	t_list	*node;
	t_list	*tmp;

	node = ft_lstnew_cmd(cmd);
	if (!node)
		return ;
	if (!*cmds)
	{
		*cmds = node;
		return ;
	}
	tmp = *cmds;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	node->prev = tmp;
}

static t_list	*ft_handle_pipe_node(t_ast_node *ast)
{
	t_list	*left;
	t_list	*right;
	t_list	*tmp;

	left = ast_to_command_list(ast->left);
	right = ast_to_command_list(ast->right);
	if (!left || !right)
	{
		if (left)
			return (left);
		return (right);
	}
	tmp = left;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = right;
	right->prev = tmp;
	return (left);
}

t_list	*ast_to_command_list(t_ast_node *ast)
{
	t_list	*cmds;

	cmds = NULL;
	if (!ast)
		return (NULL);
	if (ast->type == NODE_PIPE)
		return (ft_handle_pipe_node(ast));
	else if (ast->type == NODE_COMMAND)
		add_command_node(&cmds, (t_command *)ast->data);
	else if (ast->type == NODE_GROUP)
		return (ast_to_command_list(ast->left));
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
		return (ast_to_command_list(ast->left));
	return (cmds);
}

static int	ft_execute_logical_node(t_ast_node *ast, t_env *env)
{
	int	left_status;

	left_status = executor_from_ast(ast->left, env);
	if (ast->type == NODE_AND)
	{
		if (left_status == 0)
			return (executor_from_ast(ast->right, env));
		return (left_status);
	}
	else if (ast->type == NODE_OR)
	{
		if (left_status != 0)
			return (executor_from_ast(ast->right, env));
		return (left_status);
	}
	return (left_status);
}

int	executor_from_ast(t_ast_node *ast, t_env *env)
{
	t_list	*cmds;
	int		status;

	if (!ast)
		return (1);
	if (ast->type == NODE_AND || ast->type == NODE_OR)
		return (ft_execute_logical_node(ast, env));
	else if (ast->type == NODE_GROUP)
		return (executor_from_ast(ast->left, env));
	cmds = ast_to_command_list(ast);
	if (!cmds)
		return (1);
	status = execute_pipe(cmds, env);
	free_command_list(cmds);
	return (status);
}
