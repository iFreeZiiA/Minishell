/* ************************************************************************** */
/*                                                                            */
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

int	ft_parse_enhanced(t_shell *shell)
{
	(void)shell;
	// TODO: Implémenter le parsing amélioré
	return (0);
}

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

t_ast_node	*ft_parser(t_list *tokens, t_shell *shell)
{
	(void)tokens;
	(void)shell;
	// TODO: Implémenter le parser principal
	return (NULL);
}
