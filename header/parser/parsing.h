/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 18:01:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

/* *************************** POINT D'ENTRÉE UNIQUE ************************ */

/**
 * @brief Point d'entrée unique du module parser - Phase 6.1
 * Parse une liste de tokens et retourne un AST avec support des pipes
 * 
 * @param tokens Liste de tokens du lexer
 * @param shell Structure shell
 * @return t_ast_node* Noeud racine de l'AST ou NULL en cas d'erreur
 */
t_ast_node	*ft_parser(t_list *tokens, t_shell *shell);
t_ast_node	*ft_parser_new(t_list *tokens, t_shell *shell);

/* ****************************** OPERATORS ********************************* */

/**
 * @brief Trouve le premier token pipe dans la liste
 * 
 * @param tokens Liste de tokens
 * @return t_list* Token pipe trouvé ou NULL
 */
t_list		*ft_find_pipe_token(t_list *tokens);

/**
 * @brief Crée un nœud AST de type pipe
 * 
 * @param left Commande de gauche
 * @param right Commande de droite
 * @return t_ast_node* Nouveau nœud pipe
 */
t_ast_node	*ft_create_pipe_node(t_ast_node *left, t_ast_node *right);

/**
 * @brief Valide la syntaxe des pipes
 * 
 * @param tokens Liste de tokens
 * @return int 0 si valide, -1 sinon
 */
int			ft_validate_pipe_syntax(t_list *tokens);

/* ************************* OPÉRATEURS LOGIQUES **************************** */

/**
 * @brief Trouve le premier opérateur logique (&&, ||) dans la liste
 * 
 * @param tokens Liste de tokens
 * @return t_list* Token opérateur trouvé ou NULL
 */
t_list		*ft_find_logical_operator(t_list *tokens);

/**
 * @brief Crée un nœud AST de type AND (&&)
 * 
 * @param left Commande de gauche
 * @param right Commande de droite
 * @return t_ast_node* Nouveau nœud AND
 */
t_ast_node	*ft_create_and_node(t_ast_node *left, t_ast_node *right);

/**
 * @brief Crée un nœud AST de type OR (||)
 * 
 * @param left Commande de gauche
 * @param right Commande de droite
 * @return t_ast_node* Nouveau nœud OR
 */
t_ast_node	*ft_create_or_node(t_ast_node *left, t_ast_node *right);

/**
 * @brief Parse les opérateurs logiques avec gestion des précédences
 * 
 * @param shell Structure shell
 * @return int 0 succès, -1 erreur
 */
int			ft_parse_logical_operators(t_shell *shell);

/**
 * @brief Valide la syntaxe des opérateurs logiques
 * 
 * @param tokens Liste de tokens
 * @return bool true si valide, false sinon
 */
bool		ft_validate_logical_syntax(t_list *tokens);

/* ******************************** PIPES ************************************ */

/**
 * @brief Extrait les tokens à gauche du pipe
 * 
 * @param tokens Liste de tokens
 * @param pipe_pos Position du token pipe
 * @return t_list* Liste des tokens de gauche
 */
t_list		*ft_extract_left_tokens(t_list *tokens, t_list *pipe_pos);

/**
 * @brief Duplique un nœud de token
 * 
 * @param original Nœud original
 * @return t_list* Nouveau nœud dupliqué
 */
t_list		*ft_duplicate_token_node(t_list *original);

/**
 * @brief Ajoute un token à la fin d'une liste
 * 
 * @param list Pointeur vers la liste
 * @param new_node Nouveau nœud à ajouter
 */
void		ft_add_token_to_list(t_list **list, t_list *new_node);

/**
 * @brief Compte le nombre de pipes dans la liste
 * 
 * @param tokens Liste de tokens
 * @return int Nombre de pipes
 */
int			ft_count_pipes(t_list *tokens);

/**
 * @brief Vérifie si une liste contient uniquement des tokens valides
 * 
 * @param tokens Liste de tokens
 * @return int 1 si valide, 0 sinon
 */
int			ft_is_valid_command_sequence(t_list *tokens);

/* *************************** UTILITAIRES AST *************************** */

/**
 * @brief Libère récursivement un nœud AST et ses enfants
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
//  * @brief Valide la structure de l'AST généré
//  * 
//  * @param ast Noeud AST à valider
//  * @return int 0 si valide, -1 sinon
//  */
int			ft_validate_ast_structure(t_ast_node *ast);

/* *************************** UTILITAIRES TOKENS *************************** */

int			ft_is_word_token(t_token *token);
int			ft_is_operator_token(t_token *token);
int			ft_is_redirect_token_type(t_token_type type);
int			ft_count_tokens_by_type(t_list *tokens, t_token_type type);
t_list		*ft_find_first_token_type(t_list *tokens, t_token_type type);

/* *************************** FONCTIONS DEBUG *************************** */

void		ft_debug_tokens(t_list *tokens);
char		*ft_get_token_type_name(t_token_type type);

#endif
