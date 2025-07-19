/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_phase_5_1.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_PHASE_5_1_H
# define PARSER_PHASE_5_1_H

# include "../minishell.h"

/* *************************** POINT D'ENTRÉE UNIQUE ************************ */

/**
 * @brief Point d'entrée unique du module parser - Phase 5.1
 * Parse une liste de tokens et retourne un AST pour commandes simples
 * 
 * @param tokens Liste de tokens du lexer
 * @param shell Structure shell (pour expansion future)
 * @return t_ast_node* Noeud racine de l'AST ou NULL en cas d'erreur
 */
t_ast_node	*ft_parser(t_list *tokens, t_shell *shell);

/* *************************** UTILITAIRES AST *************************** */

/**
 * @brief Libère récursivement un noeud AST et ses enfants
 * 
 * @param node Noeud AST à libérer
 */
void		ft_free_ast_node(t_ast_node *node);

/**
 * @brief Affiche la structure de l'AST de manière simple
 * 
 * @param ast Noeud AST à afficher
 * @param depth Profondeur d'indentation
 */
void		ft_print_ast_simple(t_ast_node *ast, int depth);

/**
 * @brief Valide la structure de l'AST généré
 * 
 * @param ast Noeud AST à valider
 * @return int 0 si valide, -1 sinon
 */
int			ft_validate_ast_structure(t_ast_node *ast);

/* *************************** UTILITAIRES TOKENS *************************** */

/**
 * @brief Vérifie si un token est de type WORD
 * 
 * @param token Token à vérifier
 * @return int 1 si WORD, 0 sinon
 */
int			ft_is_word_token(t_token *token);

/**
 * @brief Vérifie si un token est un opérateur
 * 
 * @param token Token à vérifier
 * @return int 1 si opérateur, 0 sinon
 */
int			ft_is_operator_token(t_token *token);

/**
 * @brief Vérifie si un type de token est une redirection
 * 
 * @param type Type de token à vérifier
 * @return int 1 si redirection, 0 sinon
 */
int			ft_is_redirect_token_type(t_token_type type);

/**
 * @brief Compte les tokens d'un type donné dans une liste
 * 
 * @param tokens Liste de tokens
 * @param type Type de token à compter
 * @return int Nombre de tokens du type spécifié
 */
int			ft_count_tokens_by_type(t_list *tokens, t_token_type type);

/**
 * @brief Trouve le premier token d'un type donné
 * 
 * @param tokens Liste de tokens
 * @param type Type de token recherché
 * @return t_list* Premier token trouvé ou NULL
 */
t_list		*ft_find_first_token_type(t_list *tokens, t_token_type type);

#endif
