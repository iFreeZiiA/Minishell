/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 18:19:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/* Déclarations de fonctions internes */
static int	ft_validate_quotes_and_operators(t_list *tokens);
static int	ft_validate_pipes_placement(t_list *tokens);
static int	ft_validate_redirections_placement(t_list *tokens);
static int	ft_validate_logical_operators_placement(t_list *tokens);
/**
 * @brief Validation syntaxique complète conforme à bash
 * 
 * @param tokens Liste des tokens à valider
 * @return int 0 si valide, code d'erreur bash sinon
 */
int	ft_validate_syntax_bash_compliant(t_list *tokens)
{
	int	result;

	if (!tokens)
		return (0);
	result = ft_validate_quotes_and_operators(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_pipes_placement(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_redirections_placement(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_logical_operators_placement(tokens);
	if (result != 0)
		return (result);
	return (0);
}
/**
 * @brief Valide les quotes et opérateurs de base
 * 
 * @param tokens Liste des tokens
 * @return int 0 si valide, 2 si erreur de syntaxe
 */
static int	ft_validate_quotes_and_operators(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*prev_token;

	current = tokens;
	prev_token = NULL;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (!token)
			return (2);
		if (ft_check_invalid_token_sequence(prev_token, token))
		{
			ft_print_syntax_error_bash(token);
			return (2);
		}
		prev_token = token;
		current = current->next;
	}
	return (0);
}
/**
 * @brief Valide le placement des pipes
 * 
 * @param tokens Liste des tokens
 * @return int 0 si valide, 2 si erreur
 */
static int	ft_validate_pipes_placement(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command_before;

	current = tokens;
	has_command_before = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PIPE)
		{
			if (!has_command_before || !ft_has_command_after_pipe(current))
			{
				ft_printerr("bash: syntax error near unexpected token `|'\n");
				return (2);
			}
			has_command_before = 0;
		}

		else if (token->type == TOKEN_WORD)
			has_command_before = 1;
		current = current->next;
	}
	return (0);
}
/**
 * @brief Valide le placement des redirections
 * 
 * @param tokens Liste des tokens
 * @return int 0 si valide, 2 si erreur
 */
static int	ft_validate_redirections_placement(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*next_token;

	current = tokens;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (ft_is_redirection_token(token))
		{
			if (!current->next)
			{
				ft_printerr("bash: syntax error near unexpected token `newline'\n");
				return (2);
			}
			next_token = (t_token *)current->next->content.token;
			if (!next_token || next_token->type != TOKEN_WORD)
			{
				ft_print_redirection_error_bash(next_token);
				return (2);
			}
		}
		current = current->next;
	}
	return (0);
}
/**
 * @brief Valide le placement des opérateurs logiques
 * 
 * @param tokens Liste des tokens
 * @return int 0 si valide, 2 si erreur
 */
static int	ft_validate_logical_operators_placement(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command_before;

	current = tokens;
	has_command_before = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
		{
			if (!has_command_before || !ft_has_command_after_logical(current))
			{
				ft_print_logical_error_bash(token);
				return (2);
			}
			has_command_before = 0;
		}

		else if (token->type == TOKEN_WORD)
			has_command_before = 1;
		current = current->next;
	}
	return (0);
}
