/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:21:18 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 19:20:46 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Gets the type name of an AST node (for debugging)
 * 
 * @param type Node type
 * @return char* String representation of the node type
 */
static char	*ft_get_node_type_str(node_type type)
{
	if (type == NODE_COMMAND)
		return ("COMMAND");
	else if (type == NODE_PIPE)
		return ("PIPE");
	else if (type == NODE_AND)
		return ("AND");
	else if (type == NODE_OR)
		return ("OR");
	else if (type == NODE_REDIR)
		return ("REDIR");
	else if (type == NODE_GROUP)
		return ("GROUP");
	else
		return ("UNKNOWN");
}

/**
 * @brief Gets the type name of a redirection (for debugging)
 * 
 * @param type Redirection type
 * @return char* String representation of the redirection type
 */
static char	*ft_get_redir_type_str(redir_type type)
{
	if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == REDIR_HEREDOC)
		return ("REDIR_HEREDOC");
	else if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	else
		return ("UNKNOWN");
}

/**
 * @brief Frees a command structure
 * 
 * @param cmd Command to free
 */
static void	ft_free_command(t_command *cmd)
{
	int	i;
	
	if (!cmd)
		return;
		
	/* Free arguments */
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	
	/* Free redirections */
	if (cmd->redirs)
		ft_lstfree_t(cmd->redirs);
	
	free(cmd);
}

/**
 * @brief Frees an AST
 * 
 * @param ast Root of the AST
 */
void	ft_free_ast(t_ast_node *ast)
{
	if (!ast)
		return;
		
	/* Free child nodes (recursively) */
	ft_free_ast(ast->left);
	ft_free_ast(ast->right);
	
	/* Free node data based on type */
	if (ast->type == NODE_COMMAND)
		ft_free_command((t_command *)ast->data);
	else if (ast->type == NODE_REDIR)
	{
		t_redir *redir = (t_redir *)ast->data;
		if (redir)
		{
			free(redir->file);
			free(redir);
		}
	}
	
	free(ast);
}

/**
 * @brief Creates a command node in the AST
 * 
 * @param args Array of command arguments
 * @param redirs List of redirections
 * @return t_ast_node* New command node, NULL on error
 */
t_ast_node	*ft_create_command_node(char **args, t_list *redirs)
{
	t_command	*cmd;
	t_ast_node	*node;
	
	/* Create command structure */
	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	
	cmd->args = args;
	cmd->redirs = redirs;
	
	/* Create AST node */
	node = (t_ast_node *)malloc(sizeof(t_ast_node));
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
 * @brief Creates an operator node in the AST
 * 
 * @param type Type of operator (PIPE, AND, OR)
 * @param left Left child node
 * @param right Right child node
 * @return t_ast_node* New operator node, NULL on error
 */
t_ast_node	*ft_create_operator_node(node_type type, t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;
	
	if (type != NODE_PIPE && type != NODE_AND && type != NODE_OR)
		return (NULL);
	
	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	
	node->type = type;
	node->data = NULL;
	node->left = left;
	node->right = right;
	
	return (node);
}

/**
 * @brief Creates a group node in the AST
 * 
 * @param content Content of the group
 * @return t_ast_node* New group node, NULL on error
 */
t_ast_node	*ft_create_group_node(t_ast_node *content)
{
	t_ast_node	*node;
	
	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	
	node->type = NODE_GROUP;
	node->data = NULL;
	node->left = content;
	node->right = NULL;
	
	return (node);
}

/**
 * @brief Prints the redirections of a command (for debugging)
 * 
 * @param redirs List of redirections
 * @param indent Indentation level
 */
static void	ft_print_redirections(t_list *redirs, int indent)
{
	t_list	*curr;
	t_redir	*redir;
	int		i;
	
	if (!redirs)
		return;
	
	curr = redirs;
	while (curr)
	{
		if (curr->type != TYPE_REDIR)
		{
			curr = curr->next;
			continue;
		}
		
		redir = curr->content.redir;
		
		/* Print indentation */
		for (i = 0; i < indent; i++)
			ft_printf("  ");
		
		ft_printf("- %s: \"%s\" (fd: %d)\n", 
			ft_get_redir_type_str(redir->type), 
			redir->file, 
			redir->fd);
		
		curr = curr->next;
	}
}

/**
 * @brief Prints a command node (for debugging)
 * 
 * @param cmd Command to print
 * @param indent Indentation level
 */
static void	ft_print_command(t_command *cmd, int indent)
{
	int	i;
	int	j;
	
	if (!cmd)
		return;
	
	/* Print indentation */
	for (i = 0; i < indent; i++)
		ft_printf("  ");
	
	/* Print command name */
	ft_printf("COMMAND: ");
	
	/* Print arguments */
	if (cmd->args)
	{
		for (j = 0; cmd->args[j]; j++)
		{
			ft_printf("\"%s\"", cmd->args[j]);
			if (cmd->args[j + 1])
				ft_printf(" ");
		}
	}
	
	ft_printf("\n");
	
	/* Print redirections */
	if (cmd->redirs)
		ft_print_redirections(cmd->redirs, indent + 1);
}

/**
 * @brief Prints an AST for debugging purposes
 * 
 * @param ast Root of the AST
 * @param level Current level in the tree
 */
void	ft_print_ast(t_ast_node *ast, int level)
{
	int	i;
	
	if (!ast)
		return;
	
	/* Print indentation */
	for (i = 0; i < level; i++)
		ft_printf("  ");
	
	/* Print node type */
	ft_printf("%s", ft_get_node_type_str(ast->type));
	
	/* Print node details based on type */
	if (ast->type == NODE_COMMAND)
	{
		ft_printf("\n");
		ft_print_command((t_command *)ast->data, level + 1);
	}
	else if (ast->type == NODE_REDIR)
	{
		t_redir *redir = (t_redir *)ast->data;
		ft_printf(": %s \"%s\"\n", 
			ft_get_redir_type_str(redir->type), 
			redir->file);
	}
	else
	{
		ft_printf("\n");
	}
	
	/* Print child nodes */
	if (ast->left)
		ft_print_ast(ast->left, level + 1);
	if (ast->right)
		ft_print_ast(ast->right, level + 1);
}

/**
 * @brief Creates an AST from a command list
 * 
 * @param cmd_list List of commands
 * @return t_ast_node* Root of the AST, NULL on error
 */
t_ast_node	*ft_create_ast(t_list *cmd_list)
{
	t_list		*curr;
	t_ast_node	*root;
	t_ast_node	*prev_node;
	
	if (!cmd_list)
		return (NULL);
	
	root = NULL;
	prev_node = NULL;
	curr = cmd_list;
	
	while (curr)
	{
		t_ast_node *new_node = NULL;
		
		/* Create node based on type */
		if (curr->type == TYPE_COMMAND)
		{
			t_command *cmd = curr->content.cmd;
			new_node = ft_create_command_node(cmd->args, cmd->redirs);
		}
		
		/* If no root yet, set it */
		if (!root)
			root = new_node;
		/* Otherwise connect to previous node */
		else if (prev_node && new_node)
		{
			t_ast_node *pipe_node = ft_create_operator_node(NODE_PIPE, prev_node, new_node);
			if (!pipe_node)
			{
				ft_free_ast(root);
				return (NULL);
			}
			root = pipe_node;
		}
		
		prev_node = new_node;
		curr = curr->next;
	}
	
	return (root);
}

/**
 * @brief Converts an AST to a list of commands (for execution)
 * 
 * @param ast Root of the AST
 * @return t_list* List of commands, NULL on error
 */
t_list	*ast_to_command_list(t_ast_node *ast)
{
	t_list	*cmd_list;
	t_list	*left_list;
	t_list	*right_list;
	t_list	*temp;
	
	if (!ast)
		return (NULL);
	
	cmd_list = NULL;
	
	/* Handle based on node type */
	if (ast->type == NODE_PIPE)
	{
		/* Get commands from left subtree */
		left_list = ast_to_command_list(ast->left);
		if (!left_list)
			return (NULL);
		
		/* Get commands from right subtree */
		right_list = ast_to_command_list(ast->right);
		if (!right_list)
		{
			ft_lstfree_t(left_list);
			return (NULL);
		}
		
		/* Combine the lists */
		cmd_list = left_list;
		temp = ft_lstlast(cmd_list);
		temp->next = right_list;
		right_list->prev = temp;
	}
	else if (ast->type == NODE_COMMAND)
	{
		/* Create a command node */
		t_command *cmd = (t_command *)ast->data;
		t_list *new_node = ft_lstnew_cmd(cmd);
		if (!new_node)
			return (NULL);
		
		cmd_list = new_node;
	}
	
	return (cmd_list);
}
