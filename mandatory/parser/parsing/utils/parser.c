/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:38:50 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Valide la liste de tokens
 * @param tokens Liste de tokens
 * @return int 0 si valide, -1 sinon
 */
int	ft_validate_token_list(t_list *tokens)
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

/**
 * @brief Compte les tokens WORD dans la liste
 * @param tokens Liste de tokens
 * @return int Nombre de tokens WORD
 */
int	ft_count_word_tokens(t_list *tokens)
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

/**
 * @brief Extrait les arguments de commande
 * @param tokens Liste de tokens
 * @param word_count Nombre de mots
 * @return char** Tableau d'arguments
 */
char	**ft_extract_command_args(t_list *tokens, int word_count)
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

/**
 * @brief Libère structure command
 * @param cmd Commande à libérer
 */
void	ft_free_command_struct(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirs)
		ft_free_redirection_list(cmd->redirs);
	free(cmd);
}
