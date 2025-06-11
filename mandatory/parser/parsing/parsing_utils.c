/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:43:58 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Frees arguments array
 * 
 * @param args Arguments array
 * @param count Number of allocated elements
 * @return char** NULL
 */
char	**ft_free_args_array(char **args, int count)
{
	int	i;

	if (!args)
		return (NULL);
	i = 0;
	while (i < count)
	{
		free(args[i]);
		i++;
	}
	free(args);
	return (NULL);
}

/**
 * @brief Frees command structure
 * 
 * @param cmd Command to free
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
		ft_lstfree_t(cmd->redirs);
	free(cmd);
}

/**
 * @brief Validates parentheses balance in token list
 * 
 * @param token_list Token list to validate
 * @return int 1 if balanced, 0 otherwise
 */
int	ft_validate_parentheses(t_list *token_list)
{
	t_list	*curr;
	int		depth;

	curr = token_list;
	depth = 0;
	while (curr)
	{
		if (curr->content.token->type == TOKEN_PAREN_OPEN)
			depth++;
		else if (curr->content.token->type == TOKEN_PAREN_CLOSE)
		{
			depth--;
			if (depth < 0)
				return (0);
		}
		curr = curr->next;
	}
	return (depth == 0);
}

/**
 * @brief Checks if token is a redirection operator
 * 
 * @param token Token to check
 * @return int 1 if redirection, 0 otherwise
 */
int	ft_is_redirection_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_HEREDOC
		|| token->type == TOKEN_APPEND);
}

/**
 * @brief Counts tokens of specific type in range
 * 
 * @param start Start token
 * @param end End boundary
 * @param type Token type to count
 * @return int Count of matching tokens
 */
int	ft_count_tokens_of_type(t_list *start, t_list *end, t_token_type type)
{
	t_list	*curr;
	int		count;

	curr = start;
	count = 0;
	while (curr && curr != end)
	{
		if (curr->content.token->type == type)
			count++;
		curr = curr->next;
	}
	return (count);
}
