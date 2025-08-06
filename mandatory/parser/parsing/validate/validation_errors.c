/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_errors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:43:08 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static void	ft_print_token_error_message(t_token *token)
{
	char	*token_name;

	token_name = ft_get_token_name_for_error(token->type);
	ft_printerr("bash: syntax error near unexpected token `%s'\n", token_name);
}

static void	ft_print_value_error_message(t_token *token)
{
	if (token->value)
		ft_printerr("bash: syntax error near unexpected token `%s'\n",
			token->value);
	else
		ft_printerr("bash: syntax error near unexpected token\n");
}

/**
 * @brief Affiche une erreur de syntaxe conforme à bash (fonction unifiée)
 * 
 * @param token Token causant l'erreur
 */
void	ft_print_syntax_error_bash(t_token *token)
{
	if (!token)
	{
		ft_printerr("bash: syntax error near unexpected token `newline'\n");
		return ;
	}
	if (token->type == TOKEN_PIPE || token->type == TOKEN_AND
		|| token->type == TOKEN_OR || ft_is_redirection_token(token))
		ft_print_token_error_message(token);
	else
		ft_print_value_error_message(token);
}

/**
 * @brief Obtient le nom d'un token pour les messages d'erreur
 * 
 * @param type Type de token
 * @return char* Nom du token
 */
char	*ft_get_token_name_for_error(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return ("|");
	if (type == TOKEN_AND)
		return ("&&");
	if (type == TOKEN_OR)
		return ("||");
	if (type == TOKEN_REDIR_IN)
		return ("<");
	if (type == TOKEN_REDIR_OUT)
		return (">");
	if (type == TOKEN_APPEND)
		return (">>");
	if (type == TOKEN_HEREDOC)
		return ("<<");
	if (type == TOKEN_PAREN_OPEN)
		return ("(");
	if (type == TOKEN_PAREN_CLOSE)
		return (")");
	return ("token");
}

/**
 * @brief Valide les quotes dans l'entrée utilisateur
 * 
 * @param input Chaîne d'entrée
 * @return int 0 si valide, 2 si erreur
 */
int	ft_validate_quotes_in_input(char *input)
{
	int		i;
	int		in_single_quote;
	int		in_double_quote;
	char	current_char;

	if (!input)
		return (0);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		current_char = input[i];
		if (current_char == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (current_char == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	if (in_single_quote || in_double_quote)
	{
		ft_printerr("bash: syntax error: unterminated quoted string\n");
		return (2);
	}
	return (0);
}
