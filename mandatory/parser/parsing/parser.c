/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 17:00:56 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static int	ft_validate_token_list(t_list *tokens)
{
	t_list	*current;

	if (!tokens)
		return (-1);
	current = tokens;
	while (current)
	{
		if (!current->content.token)
			return (-1);
		if (!current->content.token->value)
			return (-1);
		current = current->next;
	}
	return (0);
}

static int	ft_count_word_tokens(t_list *tokens)
{
	t_list	*current;
	int		count;

	current = tokens;
	count = 0;
	while (current)
	{
		if (current->content.token->type == TOKEN_WORD)
			count++;
		current = current->next;
	}
	return (count);
}

static char	**ft_extract_command_args(t_list *tokens, int word_count)
{
	char	**args;
	t_list	*current;
	int		index;

	args = malloc(sizeof(char *) * (word_count + 1));
	if (!args)
		return (NULL);
	current = tokens;
	index = 0;
	while (current && index < word_count)
	{
		if (current->content.token->type == TOKEN_WORD)
		{
			args[index] = ft_strdup(current->content.token->value);
			if (!args[index])
			{
				while (index > 0)
					free(args[--index]);
				free(args);
				return (NULL);
			}
			index++;
		}
		current = current->next;
	}
	args[word_count] = NULL;
	return (args);
}

static t_ast_node	*ft_create_simple_command_node(char **args)
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

/**
 * @brief Point d'entrée du parser pour commandes simples
 * Parse une liste de tokens et retourne un AST
 * 
 * @param tokens Liste de tokens du lexer
 * @param shell Structure shell
 * @return t_ast_node* Noeud racine de l'AST ou NULL en cas d'erreur
 */
t_ast_node	*ft_parser(t_list *tokens, t_shell *shell)
{
	char	**args;
	int		word_count;

	(void)shell;
	if (ft_validate_token_list(tokens) != 0)
		return (NULL);
	word_count = ft_count_word_tokens(tokens);
	if (word_count == 0)
		return (NULL);
	args = ft_extract_command_args(tokens, word_count);
	if (!args)
		return (NULL);
	return (ft_create_simple_command_node(args));
}
