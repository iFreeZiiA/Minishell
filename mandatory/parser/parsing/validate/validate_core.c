/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/08 20:34:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

int	ft_val_pipes(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command;
	int		consecutive_pipes;

	current = tokens;
	has_command = 0;
	consecutive_pipes = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PIPE)
		{
			consecutive_pipes++;
			if (consecutive_pipes > 1)
				return (0);
			if (!has_command)
				return (0);
			has_command = 0;
		}
		else
		{
			consecutive_pipes = 0;
			if (token->type == TOKEN_WORD)
				has_command = 1;
		}
		current = current->next;
	}
	if (!has_command && tokens)
		return (0);
	return (1);
}

static int	ft_val_logical_ops(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command;
	int		consecutive_ops;

	current = tokens;
	has_command = 0;
	consecutive_ops = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		{
			consecutive_ops++;
			if (consecutive_ops > 1)
				return (0);
			if (!has_command)
				return (0);
			has_command = 0;
		}
		else
		{
			consecutive_ops = 0;
			if (token->type == TOKEN_WORD)
				has_command = 1;
		}
		current = current->next;
	}
	if (!has_command && tokens)
		return (0);
	return (1);
}

static int	ft_val_paren(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*prev_token;
	int		balance;

	current = tokens;
	balance = 0;
	prev_token = NULL;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
		{
			balance++;
			// Vérifier que le premier token après '(' n'est pas un opérateur
			if (current->next)
			{
				t_token *next = (t_token *)current->next->content.token;
				if (next->type == TOKEN_PIPE || next->type == TOKEN_AND || 
					next->type == TOKEN_OR || ft_is_redir_tok(next))
					return (0);
			}
		}
		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			balance--;
			if (balance < 0)
				return (0);
			// Vérifier que le token avant ')' n'est pas un opérateur
			if (prev_token && (prev_token->type == TOKEN_PIPE || 
				prev_token->type == TOKEN_AND || prev_token->type == TOKEN_OR ||
				ft_is_redir_tok(prev_token)))
				return (0);
		}
		prev_token = token;
		current = current->next;
	}
	return (balance == 0);
}

static int	ft_validate_token_size(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		total_length;

	current = tokens;
	total_length = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->value)
		{
			total_length += ft_strlen(token->value);
			// Limite de sécurité : éviter les tokens extrêmement longs
			if (ft_strlen(token->value) > 100000)
				return (0);
		}
		// Limite totale de la commande
		if (total_length > 500000)
			return (0);
		current = current->next;
	}
	return (1);
}

int	ft_val_syntax(t_list *tokens)
{
	t_token	*token;

	if (!tokens)
		return (2);
	
	// Validation de la taille pour éviter les problèmes de mémoire
	if (!ft_validate_token_size(tokens))
		return (2);
	
	// Cas spécial : un seul token qui est un opérateur
	if (!tokens->next)
	{
		token = (t_token *)tokens->content.token;
		if (ft_is_redir_tok(token) || token->type == TOKEN_PIPE ||
			token->type == TOKEN_AND || token->type == TOKEN_OR)
			return (2);
	}
	
	if (!ft_val_ops(tokens))
		return (2);
	if (!ft_val_pipes(tokens))
		return (2);
	if (!ft_val_redir(tokens))
		return (2);
	if (!ft_val_logical_ops(tokens))
		return (2);
	if (!ft_val_paren(tokens))
		return (2);
	return (0);
}
