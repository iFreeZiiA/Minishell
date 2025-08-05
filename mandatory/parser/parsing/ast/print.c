/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/02 14:20:22 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

// Forward declaration
static void	ft_print_ast_tree(t_ast_node *ast, int level);

/**
 * @brief Prints indentation spaces for tree visualization
 * 
 * This helper function prints a specified number of indentation spaces
 * to create a visual hierarchy when printing the AST tree structure.
 * 
 * @param level Number of indentation levels (each level = 2 spaces)
 */
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

/**
 * @brief Converts AST node type to its string representation
 * 
 * This function maps AST node types to their corresponding string
 * representations for debugging and visualization purposes.
 * 
 * @param type AST node type to convert
 * @return char* String representation of the node type
 */
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

/**
 * @brief Prints the arguments of a command in a formatted way
 * 
 * This helper function prints all arguments of a command with proper
 * spacing and quotation marks for visualization.
 * 
 * @param cmd Command structure containing arguments to print
 */
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

/**
 * @brief Prints the content and children of an AST node
 * 
 * This function handles the printing logic for different types of AST nodes:
 * - Command nodes: prints their arguments
 * - Group nodes: indicates subshell and prints content
 * - Operator nodes: prints left and right children
 * 
 * @param ast AST node whose content should be printed
 * @param level Current indentation level for proper formatting
 */
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

/**
 * @brief Recursively prints the entire AST tree structure
 * 
 * This function performs a recursive traversal of the AST tree and prints
 * each node with proper indentation to visualize the tree structure.
 * It handles null nodes gracefully for debugging purposes.
 * 
 * @param ast Root node of the AST tree to print
 * @param level Current indentation level (0 for root)
 */
static void	ft_print_ast_tree(t_ast_node *ast, int level)
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
