/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 17:58:56 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

t_ast_node	*ft_parse_pipe_expression(t_list *tokens, t_shell *shell);

/**
 * @brief Détecte le type de parsing nécessaire selon les tokens
 * 
 * @param tokens Liste de tokens
 * @return int Type de parsing: 1=simple, 2=pipes, 3=logical, 4=parentheses
 */
static int	ft_detect_parsing_type(t_list *tokens)
{
	t_list	*curr;
	t_token	*token;

	if (!tokens)
		return (0);
	curr = tokens;
	while (curr)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PAREN_OPEN || token->type == TOKEN_PAREN_CLOSE)
			return (4);
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
			return (3);
		if (token->type == TOKEN_PIPE)
			return (2);
		curr = curr->next;
	}
	return (1);
}



/**
 * @brief Applique le parsing approprié selon le type détecté
 * 
 * @param shell Structure shell
 * @param parsing_type Type de parsing nécessaire
 * @return int 0 succès, -1 erreur
 */
static int	ft_apply_parsing_strategy(t_shell *shell, int parsing_type)
{
	t_ast_node	*result_ast;

	result_ast = NULL;
	if (parsing_type == 4)
	{
		result_ast = ft_parse_expression(shell->token, NULL, shell);
	}
	else if (parsing_type == 3)
	{
		if (ft_parse_logical_operators(shell) != 0)
			return (-1);
		return (0);
	}
	else if (parsing_type == 2)
	{
		result_ast = ft_parse_pipe_expression(shell->token, shell);
	}
	else
	{
		result_ast = ft_parser(shell->token, shell);
	}
	if (!result_ast)
		return (-1);
	shell->ast = result_ast;
	return (0);
}

/**
 * @brief Valide les entrées avant parsing
 * 
 * @param shell Structure shell
 * @return int 0 succès, -1 erreur
 */
static int	ft_validate_parse_input(t_shell *shell)
{
	if (!shell)
		return (-1);
	if (!shell->token)
		return (-1);
	if (ft_lexer_checker(shell) != 0)
		return (-1);
	return (0);
}

/**
 * @brief Point d'entrée principal du parsing intégré
 * Utilise toutes les fonctions des phases 2-11.4 au bon moment
 * 
 * @param shell Structure shell avec tokens du lexer
 * @return int 0 succès, -1 erreur
 */
int	ft_parse(t_shell *shell)
{
	int	parsing_type;

	if (ft_validate_parse_input(shell) != 0)
		return (-1);
	if (ft_validate_logical_syntax(shell->token) == false)
		return (-1);
	parsing_type = ft_detect_parsing_type(shell->token);
	if (parsing_type == 0)
		return (-1);
	return (ft_apply_parsing_strategy(shell, parsing_type));
}
