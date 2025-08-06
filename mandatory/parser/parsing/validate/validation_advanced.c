/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_advanced.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:41:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Valide l'équilibrage des parenthèses
 * 
 * @param tokens Liste des tokens
 * @return int 0 si valide, 2 si erreur
 */
int	ft_validate_parentheses_balance(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		paren_count;

	current = tokens;
	paren_count = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			paren_count++;
		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			paren_count--;
			if (paren_count < 0)
			{
				ft_printerr("bash: syntax error near unexpected token `)'\n");
				return (2);
			}
		}
		current = current->next;
	}
	if (paren_count > 0)
	{
		ft_printerr("bash: syntax error: unexpected end of file\n");
		return (2);
	}
	return (0);
}
/**
 * @brief Valide la structure des commandes composées
 * 
 * @param tokens Liste des tokens
 * @return int 0 si valide, 2 si erreur
 */

int	ft_validate_compound_commands(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		word_count;

	current = tokens;
	word_count = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_WORD)
			word_count++;
		else if (ft_is_operator_token(token))
		{
			if (word_count == 0)
			{
				ft_print_syntax_error_bash(token);
				return (2);
			}
			word_count = 0;
		}
		current = current->next;
	}
	return (0);
}
/**
 * @brief Valide les heredocs
 * 
 * @param tokens Liste des tokens
 * @return int 0 si valide, 2 si erreur
 */

int	ft_validate_heredoc_syntax(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	t_token	*next_token;

	current = tokens;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_HEREDOC)
		{
			if (!current->next)
			{
				ft_printerr("bash: syntax error near unexpected token `newline'\n");
				return (2);
			}
			next_token = (t_token *)current->next->content.token;
			if (!next_token || next_token->type != TOKEN_WORD)
			{
				ft_printerr("bash: syntax error near unexpected token `%s'\n",
					next_token ? next_token->value : "newline");
				return (2);
			}
		}
		current = current->next;
	}
	return (0);
}
/**
 * @brief Valide la syntaxe complète - fonction principale
 * 
 * @param input Chaîne d'entrée utilisateur
 * @param tokens Liste des tokens (peut être NULL si pas encore tokenisée)
 * @return int 0 si valide, 2 si erreur de syntaxe
 */

int	ft_validate_complete_syntax(char *input, t_list *tokens)
{
	int	result;

	if (!input)
		return (0);
	result = ft_validate_quotes_in_input(input);
	if (result != 0)
		return (result);
	if (!tokens)
		return (0);
	result = ft_validate_syntax(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_parentheses_balance(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_compound_commands(tokens);
	if (result != 0)
		return (result);
	result = ft_validate_heredoc_syntax(tokens);
	if (result != 0)
		return (result);
	return (0);
}
/**
 * @brief Vérifie si une entrée est vide ou contient seulement des espaces
 * 
 * @param input Chaîne d'entrée
 * @return int 1 si vide, 0 sinon
 */

int	ft_is_empty_or_whitespace(char *input)
{
	int	i;

	if (!input)
		return (1);
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}
