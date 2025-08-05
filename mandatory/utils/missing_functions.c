/* ************************************************************************** */
/*                                        static t_command	*ft_create_command_from_args(char **args)                   */
/*                                                        :::      ::::::::   */
/*   missing_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/27 00:00:00 by jjorda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/*
 * Implémentations temporaires des fonctions manquantes
 * À remplacer par les vraies implémentations plus tard
 */

void	ft_free_arr(char **arr)
{
	int	i;
	
	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	ft_validate_token_list(t_list *tokens)
{
	(void)tokens;
	// TODO: Implémenter la validation des tokens
	return (1);
}

int	ft_count_word_tokens(t_list *tokens)
{
	int		count;
	t_list	*current;
	
	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			t_token *token = current->content.token;
			if (token && token->type == TOKEN_WORD)
				count++;
		}
		current = current->next;
	}
	return (count);
}

char	**ft_extract_command_args(t_list *tokens, int word_count)
{
	char	**args;
	int		i;
	t_list	*current;
	
	(void)word_count;  // Utiliser le paramètre word_count si nécessaire
	args = malloc(sizeof(char *) * (word_count + 1));
	if (!args)
		return (NULL);
		
	i = 0;
	current = tokens;
	while (current && i < word_count)
	{
		if (current->type == TYPE_TOKEN)
		{
			t_token *token = current->content.token;
			if (token && token->type == TOKEN_WORD)
			{
				args[i] = ft_strdup(token->value);
				if (!args[i])
				{
					ft_free_arr(args);
					return (NULL);
				}
				i++;
			}
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

t_ast_node	*ft_create_simple_command_node(char **args)
{
	t_ast_node	*node;
	t_command	*cmd;
	
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
		
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		free(node);
		return (NULL);
	}
	
	cmd->args = args;
	cmd->redirs = NULL;
	
	node->type = NODE_COMMAND;
	node->data = cmd;
	node->left = NULL;
	node->right = NULL;
	
	return (node);
}

static int	ft_validate_input(t_shell *shell)
{
	if (!shell)
		return (-1);
	if (!shell->token)
		return (-1);
	return (0);
}

static t_command	*ft_create_command_from_args(char **args)
{
	t_command	*cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = args;
	cmd->redirs = NULL;
	return (cmd);
}

static t_ast_node	*ft_create_ast_node(t_command *cmd)
{
	t_ast_node	*node;
	
	node = malloc(sizeof(t_ast_node));
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

t_ast_node	*ft_parser(t_list *tokens, t_shell *shell)
{
	char		**args;
	t_command	*cmd;
	t_ast_node	*ast;
	int			count;
	
	if (ft_validate_input(shell) != 0)
		return (NULL);
	count = ft_count_word_tokens(tokens);
	if (count == 0)
		return (NULL);
	args = ft_extract_command_args(tokens, count);
	if (!args)
		return (NULL);
	cmd = ft_create_command_from_args(args);
	if (!cmd)
		return (NULL);
	ast = ft_create_ast_node(cmd);
	return (ast);
}

void	free_command_list(t_list *cmds)
{
	t_list	*current;
	t_list	*next;
	
	current = cmds;
	while (current)
	{
		next = current->next;
		if (current->content.cmd)
		{
			if (current->content.cmd->args)
				ft_free_arr(current->content.cmd->args);
			free(current->content.cmd);
		}
		free(current);
		current = next;
	}
}

int	ft_parse_input_redirection(t_list *tokens, t_command *cmd)
{
	(void)tokens;
	(void)cmd;
	return (0);
}

int	ft_parse_output_redirection(t_list *tokens, t_command *cmd)
{
	(void)tokens;
	(void)cmd;
	return (0);
}

int	ft_parse_heredoc_redirection(t_list *tokens, t_command *cmd)
{
	(void)tokens;
	(void)cmd;
	return (0);
}

int	ft_parse_append_redirection(t_list *tokens, t_command *cmd)
{
	(void)tokens;
	(void)cmd;
	return (0);
}

int	ft_expand_wildcard(t_shell *shell, t_list **tok_h)
{
	(void)shell;
	(void)tok_h;
	return (0);
}

bool	ft_is_logical_operator_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		return (true);
	return (false);
}

int	ft_validate_syntax(t_list *tokens)
{
	if (!tokens)
		return (0);  // Toujours valide pour les cas simples
	return (1);      // 1 = valide selon le code parse.c
}

bool	ft_validate_logical_syntax(t_list *token_h)
{
	if (!token_h)
		return (false);
	return (true);
}

t_ast_node	*ft_parse_logical_expression(t_shell *shell, t_list *start, t_list *end)
{
	(void)shell;
	(void)start;
	(void)end;
	return (NULL);
}

t_ast_node	*ft_parse_pipe_expression(t_list *tokens, t_shell *shell)
{
	(void)tokens;
	(void)shell;
	return (NULL);
}
