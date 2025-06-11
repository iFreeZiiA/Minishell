/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:43:41 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Counts command arguments
 * 
 * @param start First token
 * @param end Last token boundary
 * @return int Argument count
 */
static int	ft_count_command_args(t_list *start, t_list *end)
{
	t_list	*curr;
	int		count;

	curr = start;
	count = 0;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_WORD)
			count++;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Allocates and fills command arguments array
 * 
 * @param start First token
 * @param end Last token boundary
 * @param arg_count Number of arguments
 * @return char** Arguments array or NULL on error
 */
static char	**ft_fill_command_args(t_list *start, t_list *end, int arg_count)
{
	char	**args;
	t_list	*curr;
	int		i;

	args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	curr = start;
	i = 0;
	while (curr && curr != end && i < arg_count)
	{
		if (curr->content.token->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(curr->content.token->value);
			if (!args[i])
				return (ft_free_args_array(args, i));
			i++;
		}
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}

/**
 * @brief Creates command structure from tokens
 * 
 * @param shell Shell structure
 * @param start First token
 * @param end Last token boundary
 * @return t_command* Command structure or NULL on error
 */
static t_command	*ft_create_command_struct(t_shell *shell,
		t_list *start, t_list *end)
{
	t_command	*cmd;
	int			arg_count;

	if (!shell || !start)
		return (NULL);
	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	arg_count = ft_count_command_args(start, end);
	cmd->args = ft_fill_command_args(start, end, arg_count);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redirs = NULL;
	return (cmd);
}

/**
 * @brief Parses simple command from tokens
 * 
 * @param shell Shell structure
 * @param start First token
 * @param end Last token boundary
 * @return t_ast_node* Command AST node or NULL on error
 */
t_ast_node	*ft_parse_simple_command(t_shell *shell,
		t_list *start, t_list *end)
{
	t_command	*cmd;

	cmd = ft_create_command_struct(shell, start, end);
	if (!cmd)
		return (NULL);
	if (ft_parse_redirections(shell, start, cmd) < 0)
	{
		ft_free_command_struct(cmd);
		return (NULL);
	}
	return (ft_create_command_ast(cmd));
}

/**
 * @brief Parses group (parenthesized) expression
 * 
 * @param shell Shell structure
 * @param start Opening parenthesis
 * @param end Closing parenthesis
 * @return t_ast_node* Group AST node or NULL on error
 */
t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end)
{
	t_ast_node	*content;

	if (!shell || !start || !end)
		return (NULL);
	if (start->content.token->type != TOKEN_PAREN_OPEN)
		return (NULL);
	if (end->content.token->type != TOKEN_PAREN_CLOSE)
		return (NULL);
	content = ft_parse_expression(shell, start, start->next, end);
	if (!content)
		return (NULL);
	return (ft_create_group_ast(content));
}
