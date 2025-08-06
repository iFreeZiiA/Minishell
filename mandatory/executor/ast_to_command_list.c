/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_to_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:15:34 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/06 23:27:36 by jjorda           ###   ########.fr       */
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

t_list	*ast_to_command_list(t_ast_node *ast)
{
	t_list	*cmds;
	t_list	*left;
	t_list	*right;
	t_list	*tmp;

	cmds = NULL;
	if (!ast)
		return (NULL);
	if (ast->type == NODE_PIPE)
	{
		left = ast_to_command_list(ast->left);
		right = ast_to_command_list(ast->right);
		if (!left || !right)
			return (left ? left : right);
		tmp = left;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = right;
		right->prev = tmp;
		return (left);
	}

	else if (ast->type == NODE_COMMAND)
	{
		add_command_node(&cmds, (t_command *)ast->data);
	}

	else if (ast->type == NODE_GROUP)
	{
		// Pour les parenthèses, traiter le contenu du groupe
		return (ast_to_command_list(ast->left));
	}

	else if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		// Pour && et ||, on ne convertit que la première commande
		// L'execution conditionnelle sera gérée par executor_from_ast
		return (ast_to_command_list(ast->left));
	}
	return (cmds);
}

int	executor_from_ast(t_ast_node *ast, t_env *env)
{
	t_list	*cmds;
	int		status;
	int		left_status;

	// printf("DEBUG: executor_from_ast called with node type = %d\n", ast ? ast->type : -1);
	if (!ast)
	{
		// printf("DEBUG: ast is NULL\n");
		return (1);
	}
	// Gestion des opérateurs logiques && et ||
	if (ast->type == NODE_AND)
	{
		// printf("DEBUG: Executing NODE_AND\n");
		left_status = executor_from_ast(ast->left, env);
		// printf("DEBUG: NODE_AND left_status = %d\n", left_status);
		if (left_status == 0)
		{
			// printf("DEBUG: NODE_AND executing right\n");
			return (executor_from_ast(ast->right, env));
		}
		return (left_status);
	}

	else if (ast->type == NODE_OR)
	{
		// printf("DEBUG: Executing NODE_OR\n");
		left_status = executor_from_ast(ast->left, env);
		// printf("DEBUG: NODE_OR left_status = %d\n", left_status);
		if (left_status != 0)
		{
			// printf("DEBUG: NODE_OR executing right\n");
			return (executor_from_ast(ast->right, env));
		}
		return (left_status);
	}

	else if (ast->type == NODE_GROUP)
	{
		// printf("DEBUG: Executing NODE_GROUP\n");
		// Pour les parenthèses, exécuter le contenu du groupe
		return (executor_from_ast(ast->left, env));
	}
	// printf("DEBUG: Using standard execution path for type %d\n", ast->type);
	// Gestion standard pour pipes et commandes simples
	cmds = ast_to_command_list(ast);
	if (!cmds)
	{
		// printf("DEBUG: ast_to_command_list returned NULL\n");
		return (1);
	}
	status = execute_pipe(cmds, env);
	// printf("DEBUG: execute_pipe returned status = %d\n", status);
	free_command_list(cmds);
	return (status);
}
