/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Frees an array of command arguments
 * 
 * This helper function deallocates memory for each string in the arguments
 * array and then frees the array itself.
 * 
 * @param args Array of strings to free
 */
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

/**
 * @brief Frees a linked list of redirections
 * 
 * This helper function traverses a linked list of redirections and
 * deallocates memory for each redirection structure and its associated data.
 * 
 * @param redirs Linked list of redirections to free
 */
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

/**
 * @brief Frees a command structure and all its associated data
 * 
 * This function deallocates memory for a command structure including
 * its arguments array and redirections list.
 * 
 * @param cmd Command structure to free
 */
void	ft_free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	ft_free_args_array(cmd->args);
	ft_free_redirections_list(cmd->redirs);
	free(cmd);
}

/**
 * @brief Frees the data associated with an AST node based on its type
 * 
 * This helper function determines the type of data stored in an AST node
 * and frees it appropriately. Currently handles command nodes.
 * 
 * @param ast AST node whose data should be freed
 */
static void	ft_free_node_data(t_ast_node *ast)
{
	if (ast->type == NODE_COMMAND && ast->data)
		ft_free_command((t_command *)ast->data);
}

/**
 * @brief Recursively frees an entire AST tree structure
 * 
 * This function performs a post-order traversal of the AST tree,
 * freeing all child nodes before freeing the current node and its data.
 * 
 * @param ast Root node of the AST tree to free
 */
void	ft_free_ast(t_ast_node *ast)
{
	if (!ast)
		return ;
	ft_free_ast(ast->left);
	ft_free_ast(ast->right);
	ft_free_node_data(ast);
	free(ast);
}
