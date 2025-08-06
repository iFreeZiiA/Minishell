/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 18:01:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/**
 * @brief Nettoie les ressources en cas d'erreur
 * 
 * @param shell Structure shell
 * @param tokens Liste de tokens à libérer
 */
static void	ft_cleanup_parsing_error(t_shell *shell, t_list *tokens)
{
	if (tokens)
		ft_lstfree_t(tokens);
	if (shell && shell->ast)
	{
		ft_free_ast_node(shell->ast);
		shell->ast = NULL;
	}
}
/**
 * @brief Valide l'input avant traitement
 * 
 * @param input Ligne de commande
 * @param shell Structure shell
 * @return int 0 succès, -1 erreur
 */
static int	ft_validate_input(char *input, t_shell *shell)
{
	if (!input || !shell)
		return (-1);
	if (ft_strlen(input) == 0)
		return (-1);
	return (0);
}
/**
 * @brief Point d'entrée unifié pour lexing + parsing complet
 * Utilise toutes les fonctions des phases 2-11.4 dans le bon ordre
 * 
 * @param input Ligne de commande à parser
 * @param shell Structure shell
 * @return int 0 succès, -1 erreur
 */

int	ft_parse_input(char *input, t_shell *shell)
{
	t_list	*tokens;

	if (ft_validate_input(input, shell) != 0)
		return (-1);
	shell->current_line = input;
	tokens = ft_lexing_new(shell);
	if (!tokens)
	{
		shell->ast = NULL;
		return (-1);
	}
	// Debug: afficher les tokens avec détails
	// ft_debug_tokens(tokens);
	shell->token = tokens;
	if (ft_parse_enhanced(shell) != 0)
	{
		// printf("DEBUG: ft_parse_enhanced FAILED\n");
		ft_cleanup_parsing_error(shell, tokens);
		return (-1);
	}
	// printf("DEBUG: ft_parse_enhanced SUCCESS\n");
	return (0);
}
/**
 * @brief Fonction alternative pour usage dans le main existant
 * Garde la compatibilité avec l'interface actuelle
 * 
 * @param shell Structure shell initialisée
 * @return int 0 succès, -1 erreur
 */

int	ft_process_command_line(t_shell *shell)
{
	if (!shell || !shell->current_line)
		return (-1);
	return (ft_parse_input(shell->current_line, shell));
}
