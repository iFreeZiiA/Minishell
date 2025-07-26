/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integration.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 22:41:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTEGRATION_H
# define INTEGRATION_H

# include "../minishell.h"

/* *************************** FONCTIONS D'INTÉGRATION ********************* */

/**
 * @brief Point d'entrée unifié pour lexing + parsing complet
 * Utilise toutes les fonctions des phases 2-11.4 dans le bon ordre
 * 
 * @param input Ligne de commande à parser
 * @param shell Structure shell
 * @return int 0 succès, -1 erreur
 */
int			ft_parse_input(char *input, t_shell *shell);

/**
 * @brief Fonction alternative pour usage dans le main existant
 * Garde la compatibilité avec l'interface actuelle
 * 
 * @param shell Structure shell initialisée
 * @return int 0 succès, -1 erreur
 */
int			ft_process_command_line(t_shell *shell);

/**
 * @brief Parser principal amélioré utilisant toutes les fonctions
 * Remplace l'ancienne version de ft_parser pour être plus complet
 * 
 * @param tokens Liste de tokens du lexer
 * @param shell Structure shell
 * @return t_ast_node* Noeud racine de l'AST ou NULL en cas d'erreur
 */
t_ast_node	*ft_enhanced_parser(t_list *tokens, t_shell *shell);

/* *************************** FONCTIONS EXISTANTES ************************ */

/**
 * @brief Point d'entrée principal du parsing intégré (amélioré)
 * Utilise toutes les fonctions des phases 2-11.4 au bon moment
 * 
 * @param shell Structure shell avec tokens du lexer
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_enhanced(t_shell *shell);

/* *************************** FONCTIONS UTILITAIRES ********************** */

/**
 * @brief Valide la liste de tokens pour toutes les phases
 * 
 * @param tokens Liste de tokens
 * @return int 0 si valide, -1 sinon
 */
int			ft_validate_token_list(t_list *tokens);

/**
 * @brief Compte les tokens WORD dans la liste
 * 
 * @param tokens Liste de tokens
 * @return int Nombre de tokens WORD
 */
int			ft_count_word_tokens(t_list *tokens);

/**
 * @brief Extrait les arguments de commande
 * 
 * @param tokens Liste de tokens
 * @param word_count Nombre de mots
 * @return char** Tableau d'arguments
 */
char		**ft_extract_command_args(t_list *tokens, int word_count);

t_ast_node	*ft_create_simple_command_node(char **args);

t_ast_node	*ft_parse_expression(t_list *start, t_list *end, t_shell *shell);

#endif
