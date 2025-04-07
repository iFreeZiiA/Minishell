/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:45:58 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/07 15:55:53 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

#ifndef STRUCT_H
# define STRUCT_H

/* --------------------------------- LEXING --------------------------------- */

typedef enum {
    TOKEN_WORD,			// Cmd, args, files ...
    TOKEN_OR,			// ||
    TOKEN_APPEND,		// >>
    TOKEN_HEREDOC,		// <<
    TOKEN_AND,			// &&
	TOKEN_STATUS,		// $?
	TOKEN_ERROR,		// In case or error
    TOKEN_PIPE,			// |
    TOKEN_REDIR_OUT,	// >
    TOKEN_REDIR_IN,		// <
	TOKEN_VAR,			// $
	TOKEN_QUOTE,		// '
	TOKEN_DQUOTE,		// "
    TOKEN_PAREN_OPEN,	// (
    TOKEN_PAREN_CLOSE,	// )
    TOKEN_WILDCARD,		// *
	TOKEN_ASSIGN,		// =
    TOKEN_EOF			// End of Line
} token_type;

typedef struct s_token {
    token_type type;
    char *value;
} t_token;

/* -------------------------------- PARSING --------------------------------- */

typedef enum {
    NODE_COMMAND,
    NODE_PIPE,
    NODE_AND,
    NODE_OR,
    NODE_REDIR,
    NODE_GROUP      // Pour les parenthèses
} node_type;

typedef struct s_ast_node {
    node_type type;
    void *data;      // Données spécifiques au type de nœud
    struct s_ast_node *left;
    struct s_ast_node *right;
} t_ast_node;

/* --------------------------------- REDIR ---------------------------------- */

typedef enum {
    REDIR_IN,        // 
    REDIR_OUT,       // >
    REDIR_APPEND,    // >>
    REDIR_HEREDOC    // 
} redir_type;

typedef struct s_redir {
    redir_type type;
    char *file;      // Nom du fichier ou délimiteur pour heredoc
    int fd;          // File descriptor
    struct s_redir *next;
} t_redir;

/* -------------------------------- COMMAND --------------------------------- */

typedef struct s_command {
    char **args;     // Tableau d'arguments (args[0] est la commande)
    t_redir *redirs; // Liste des redirections
} t_command;

/* ---------------------------------- ENV ----------------------------------- */

typedef struct s_env {
    char **env_vars;     // Variables d'environnement actuelles
    int last_exit_code;  // Stocke la valeur de $?
} t_env;

/* -------------------------------- SHELL --------------------------------- */

typedef struct s_shell {
    t_env *env;
    char *current_line;
    int signal_received; // La seule variable globale pour les signaux
    char *prompt;
    t_list	*token;     // Résultat du lexer
    t_ast_node *ast;     // Arbre syntaxique après parsing
} t_shell;

#endif