/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/26 00:00:00 by user              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Validates quotes are properly closed
 * 
 * @param tokens Token list to validate
 * @return int 1 if valid, 0 if invalid
 */
// static int	ft_validate_quotes(t_list *tokens)
// {
// 	(void)tokens;
// 	return (1);
// }
/**
 * @brief Validates operator placement in sequence
 * 
 * @param tokens Token list to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	ft_validate_operators(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*next_token;

	current = tokens;
	while (current && current->next)
	{
		token = (t_token *)current->content.token;
		next_token = (t_token *)current->next->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		{
			if (next_token->type == TOKEN_AND || next_token->type == TOKEN_OR
				|| next_token->type == TOKEN_PIPE)
				return (0);
		}
		if (token->type == TOKEN_PIPE)
		{
			if (next_token->type == TOKEN_AND || next_token->type == TOKEN_OR
				|| next_token->type == TOKEN_PIPE)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
/**
 * @brief Validates pipe tokens in sequence
 * 
 * @param tokens Token list to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	ft_validate_pipes(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command;

	current = tokens;
	has_command = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PIPE)
		{
			if (!has_command)
				return (0);
			has_command = 0;
		}

		else if (token->type == TOKEN_WORD)
			has_command = 1;
		current = current->next;
	}
	if (!has_command && tokens)
		return (0);
	return (1);
}
/**
 * @brief Validates redirection token placement
 * 
 * @param tokens Token list to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	ft_validate_redirections(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*next_token;

	if (!tokens)
		return (1);
	current = tokens;
	token = (t_token *)current->content.token;
	if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
		return (0);
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
			|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
		{
			if (!current->next)
				return (0);
			next_token = (t_token *)current->next->content.token;
			if (next_token->type != TOKEN_WORD)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
/**
 * @brief Validates logical operator placement
 * 
 * @param tokens Token list to validate
 * @return int 1 if valid, 0 if invalid
 */
static int	ft_validate_logical_ops(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command;

	current = tokens;
	has_command = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		{
			if (!has_command)
				return (0);
			has_command = 0;
		}

		else if (token->type == TOKEN_WORD)
			has_command = 1;
		current = current->next;
	}
	if (!has_command && tokens)
		return (0);
	return (1);
}
/**
 * @brief Validates parentheses balance in token sequence
 * 
 * @param tokens Token list to validate
 * @return int 1 if balanced, 0 if unbalanced
 */
static int	ft_validate_parentheses(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		balance;

	current = tokens;
	balance = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			balance++;

		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			balance--;
			if (balance < 0)
				return (0);
		}
		current = current->next;
	}
	return (balance == 0);
}
/**
 * @brief Main validation function for syntax checking
 * 
 * @param tokens Token list to validate
 * @return int 1 if syntax is valid, 0 if invalid
 */

int	ft_validate_syntax(t_list *tokens)
{
	if (!tokens)
		return (1);
	if (!ft_validate_operators(tokens))
		return (0);
	if (!ft_validate_pipes(tokens))
		return (0);
	if (!ft_validate_redirections(tokens))
		return (0);
	if (!ft_validate_logical_ops(tokens))
		return (0);
	if (!ft_validate_parentheses(tokens))
		return (0);
	return (1);
}
