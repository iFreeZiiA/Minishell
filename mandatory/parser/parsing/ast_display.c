/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_display.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:44:50 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Prints indentation for AST display
 * 
 * @param level Indentation level
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
 * @brief Gets string representation of node type
 * 
 * @param type Node type
 * @return char* String representation
 */
static char	*ft_get_node_type_string(node_type type)
{
	if (type == NODE_COMMAND)
		return ("COMMAND");
	else if (type == NODE_PIPE)
		return ("PIPE");
	else if (type == NODE_AND)
		return ("AND");
	else if (type == NODE_OR)
		return ("OR");
	else if (type == NODE_GROUP)
		return ("GROUP");
	else
		return ("UNKNOWN");
}

/**
 * @brief Prints command arguments
 * 
 * @param cmd Command structure
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
 * @brief Prints token list for debugging
 * 
 * @param token_list Token list to print
 */
void	ft_print_token_list(t_list *token_list)
{
	t_list	*curr;
	int		index;

	curr = token_list;
	index = 0;
	ft_printf("Tokens trouvés:\n");
	while (curr)
	{
		if (curr->type == TYPE_TOKEN)
		{
			ft_printf("[%d] Type: %d, Value: \"%s\"\n",
				index, curr->content.token->type,
				curr->content.token->value);
		}
		curr = curr->next;
		index++;
	}
	ft_printf("\n");
}

/**
 * @brief Prints AST structure recursively
 * 
 * @param ast AST node to print
 * @param level Current indentation level
 */
void	ft_print_ast(t_ast_node *ast, int level)
{
	if (!ast)
		return ;
	ft_print_indent(level);
	ft_printf("%s", ft_get_node_type_string(ast->type));
	if (ast->type == NODE_COMMAND)
	{
		ft_printf(": ");
		ft_print_command_args((t_command *)ast->data);
	}
	ft_printf("\n");
	if (ast->left)
		ft_print_ast(ast->left, level + 1);
	if (ast->right)
		ft_print_ast(ast->right, level + 1);
}
