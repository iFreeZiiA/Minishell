/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/26 00:00:00 by user              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

// Déclaration de la fonction
int	ft_parse_enhanced(t_shell *shell);
/**
 * @brief Detecte le type de parsing necessaire
 * 
 * @param tokens Liste de tokens
 * @return int Type de parsing (1=simple, 2=pipe, 3=logique)
 */
static int	ft_detect_parsing_type(t_list *tokens)
{
	t_list	*current;
	t_token	*token;

	current = tokens;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
			return (3);
		else if (token->type == TOKEN_PIPE)
			return (2);
		current = current->next;
	}
	return (1);
}
/**
 * @brief Applique la strategie de parsing appropriee
 * 
 * @param shell Structure shell
 * @param parsing_type Type de parsing detecte
 * @return int 0 succes, -1 erreur
 */
static int	ft_apply_parsing_strategy(t_shell *shell, int parsing_type)
{
	t_ast_node	*result_ast;
	t_list		*end_token;

	// printf("DEBUG: Parsing type detected = %d\n", parsing_type);
	end_token = shell->token;
	while (end_token && end_token->next)
		end_token = end_token->next;
	if (parsing_type == 3)
	{
		// printf("DEBUG: Using logical parsing\n");
		result_ast = ft_parse_logical_expression(shell, shell->token, end_token);
	}

	else if (parsing_type == 2)
	{
		// printf("DEBUG: Using pipe parsing\n");
		result_ast = ft_parse_pipe_expression(shell->token, shell);
	}
	else
	{
		// printf("DEBUG: Using simple parsing\n");
		result_ast = ft_parser(shell->token, shell);
	}
	// printf("DEBUG: result_ast = %p\n", result_ast);
	if (!result_ast)
		return (-1);
	shell->ast = result_ast;
	// printf("DEBUG: shell->ast assigned = %p\n", shell->ast);
	return (0);
}
/**
 * @brief Valide les quotes dans l'input brut
 * 
 * @param input Ligne de commande brute
 * @return int 1 si valide, 0 si invalide
 */
static int	ft_validate_raw_quotes(char *input)
{
	int	i;
	int	in_single;
	int	in_double;

	if (!input)
		return (1);
	i = 0;
	in_single = 0;
	in_double = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double)
			in_single = !in_single;

		else if (input[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	if (in_single || in_double)
		return (0);
	return (1);
}
/**
 * @brief Valide les entrees avant parsing
 * 
 * @param shell Structure shell
 * @return int 0 succes, -1 erreur
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
 * @brief Point d'entree principal du parsing integre ameliore
 * Utilise toutes les fonctions des phases 2-11.4 au bon moment
 * 
 * @param shell Structure shell avec tokens du lexer
 * @return int 0 succes, -1 erreur
 */

int	ft_parse_enhanced(t_shell *shell)
{
	int	parsing_type;

	if (ft_validate_parse_input(shell) != 0)
		return (-1);
	if (!ft_validate_raw_quotes(shell->current_line))
		return (-1);
	if (!ft_validate_syntax(shell->token))
		return (-1);
	if (ft_validate_logical_syntax(shell->token) == false)
		return (-1);
	parsing_type = ft_detect_parsing_type(shell->token);
	if (parsing_type == 0)
		return (-1);
	return (ft_apply_parsing_strategy(shell, parsing_type));
}
