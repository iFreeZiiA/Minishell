/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:13:56 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	ft_free_args_array(char **args)
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

static void	ft_free_redirections_list(t_list *redirs)
{
	t_list	*curr;
	t_list	*next;

	curr = redirs;
	while (curr)
	{
		next = curr->next;
		if (curr->content.redir)
		{
			free(curr->content.redir->file);
			free(curr->content.redir);
		}
		free(curr);
		curr = next;
	}
}

void	ft_free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	ft_free_args_array(cmd->args);
	ft_free_redirections_list(cmd->redirs);
	free(cmd);
}

static void	ft_free_node_data(t_ast_node *ast)
{
	if (ast->type == NODE_COMMAND && ast->data)
		ft_free_command((t_command *)ast->data);
}

void	ft_free_ast(t_ast_node *ast)
{
	if (!ast)
		return ;
	ft_free_ast(ast->left);
	ft_free_ast(ast->right);
	ft_free_node_data(ast);
	free(ast);
}
