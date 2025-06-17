/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Prints indentation spaces for hierarchical debug output
 * 
 * This helper function prints a specified number of indentation spaces
 * to create visual hierarchy in debug output for logical expressions.
 * 
 * @param level Indentation level (each level = 2 spaces)
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
 * @brief Gets string representation of logical operator for display
 * 
 * This function converts AST node types for logical operators into
 * their corresponding string symbols for debugging and visualization.
 * 
 * @param type AST node type representing a logical operator
 * @return char* String representation of the operator symbol
 */
char	*ft_get_logical_op_str(node_type type)
{
	if (type == NODE_AND)
		return ("&&");
	else if (type == NODE_OR)
		return ("||");
	else if (type == NODE_PIPE)
		return ("|");
	else
		return ("UNKNOWN");
}

/**
 * @brief Prints an operator node with its left and right subtrees
 * 
 * This function displays an operator node by printing the operator symbol
 * and recursively printing its left and right child nodes with proper
 * indentation to show the tree structure.
 * 
 * @param node Operator AST node to print
 * @param level Current indentation level for formatting
 */
static void	ft_print_operator_node(t_ast_node *node, int level)
{
	ft_printf("OPERATOR: %s\n", ft_get_logical_op_str(node->type));
	if (node->left)
	{
		ft_print_indent(level + 1);
		ft_printf("LEFT:\n");
		ft_print_logical_ast(node->left, level + 2);
	}
	if (node->right)
	{
		ft_print_indent(level + 1);
		ft_printf("RIGHT:\n");
		ft_print_logical_ast(node->right, level + 2);
	}
}

/**
 * @brief Prints a command node showing all its arguments
 * 
 * This function displays a command node by printing all its arguments
 * in a readable format with proper quoting and spacing.
 * 
 * @param node Command AST node to print
 */
static void	ft_print_command_node(t_ast_node *node)
{
	t_command	*cmd;
	int			i;

	cmd = (t_command *)node->data;
	ft_printf("COMMAND: ");
	if (cmd && cmd->args && cmd->args[0])
	{
		i = 0;
		while (cmd->args[i])
		{
			ft_printf("'%s'", cmd->args[i]);
			if (cmd->args[i + 1])
				ft_printf(" ");
			i++;
		}
	}
	else
		ft_printf("(empty)");
	
	ft_printf("\n");
}

/**
 * @brief Prints a group node representing a subshell expression
 * 
 * This function displays a group node (parenthesized expression) and
 * recursively prints its content with increased indentation.
 * 
 * @param node Group AST node to print
 * @param level Current indentation level for formatting
 */
static void	ft_print_group_node(t_ast_node *node, int level)
{
	ft_printf("GROUP (subshell):\n");
	if (node->left)
		ft_print_logical_ast(node->left, level + 1);
}

/**
 * @brief Recursively prints the logical AST tree structure
 * 
 * This function performs a recursive traversal of the logical AST and
 * prints each node with appropriate formatting based on its type:
 * - Operator nodes: shows operator symbol and left/right subtrees
 * - Command nodes: shows command arguments
 * - Group nodes: shows subshell content
 * 
 * @param node AST node to print (root of subtree)
 * @param level Current indentation level for proper formatting
 */
void	ft_print_logical_ast(t_ast_node *node, int level)
{
	if (!node)
		return;
	
	ft_print_indent(level);
	
	if (node->type == NODE_AND || node->type == NODE_OR || node->type == NODE_PIPE)
		ft_print_operator_node(node, level);
	else if (node->type == NODE_COMMAND)
		ft_print_command_node(node);
	else if (node->type == NODE_GROUP)
		ft_print_group_node(node, level);
	else
		ft_printf("NODE_TYPE: %d\n", node->type);
}

/**
 * @brief Prints comprehensive analysis of logical expression parsing
 * 
 * This function provides a complete analysis of logical expression parsing
 * including operator statistics, syntax validation status, and the resulting
 * AST structure. Used for debugging complex logical expressions.
 * 
 * @param shell Shell structure containing tokens and AST to analyze
 */
void	ft_print_logical_analysis(t_shell *shell)
{
	if (!shell)
		return;
	ft_printf("=== LOGICAL EXPRESSION ANALYSIS ===\n");
	if (!shell->token)
	{
		ft_printf("No tokens to analyze\n");
		return;
	}
	ft_debug_logical_operators(shell);
	if (shell->ast)
	{
		ft_printf("=== AST STRUCTURE ===\n");
		ft_print_logical_ast(shell->ast, 0);
		ft_printf("\n");
	}
	else
		ft_printf("No AST generated\n\n");
	ft_printf("=====================================\n");
}

/**
 * @brief Explains operator precedence rules for debugging and education
 * 
 * This function displays the operator precedence rules used in logical
 * expression parsing, helping users understand how complex expressions
 * are evaluated and grouped.
 */
void	ft_explain_operator_precedence(void)
{
	ft_printf("=== OPERATOR PRECEDENCE ===\n");
	ft_printf("1. | (PIPE)     - Highest precedence\n");
	ft_printf("2. && (AND)     - Medium precedence\n");
	ft_printf("3. || (OR)      - Lowest precedence\n");
	ft_printf("\nEvaluation: left to right at same precedence level\n");
	ft_printf("===========================\n\n");
}
