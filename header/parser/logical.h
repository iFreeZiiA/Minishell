/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 17:55:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGICAL_H
# define LOGICAL_H

# include "../minishell.h"

/* ----------------------------- LOGICAL PARSING ----------------------------- */

/**
 * @brief Main logical parsing entry point
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
int			ft_parse_logical_operators(t_shell *shell);

/**
 * @brief Validates logical operator syntax
 * 
 * @param token_h Head of token list
 * @return bool true if syntax is valid
 */
bool		ft_validate_logical_syntax(t_list *token_h);

/**
 * @brief Checks if token is a logical operator
 * 
 * @param token Token to check
 * @return bool true if logical operator
 */
bool		ft_is_logical_operator_token(t_token *token);

/**
 * @brief Gets operator precedence for parsing
 * 
 * @param type Token type
 * @return int Precedence level (higher = more priority)
 */
int			ft_get_operator_precedence(t_token_type type);

/* --------------------------- LOGICAL PARSING UTILS ------------------------- */

/**
 * @brief Finds operator with specific precedence
 * 
 * @param start Start of token range
 * @param end End of token range
 * @param precedence Target precedence level
 * @return t_list* Operator token or NULL
 */
t_list		*ft_find_operator_by_precedence(t_list *start, t_list *end, int precedence);

/**
 * @brief Converts token type to AST node type
 * 
 * @param token_type Token type to convert
 * @return node_type Corresponding AST node type
 */
node_type	ft_token_to_node_type(t_token_type token_type);

/**
 * @brief Parses expression with operator precedence
 * 
 * @param shell Shell structure
 * @param start Start token
 * @param end End token
 * @return t_ast_node* Parsed expression AST
 */
t_ast_node	*ft_parse_logical_expression(t_shell *shell, t_list *start, t_list *end);

/* --------------------------- LOGICAL PRINT UTILS --------------------------- */

/**
 * @brief Prints logical AST for debugging
 * 
 * @param node AST node to print
 * @param level Indentation level
 */
void		ft_print_logical_ast(t_ast_node *node, int level);

/**
 * @brief Gets string representation of logical operator
 * 
 * @param type AST node type
 * @return char* String representation
 */
char		*ft_get_logical_op_str(node_type type);

/**
 * @brief Validates logical AST structure
 * 
 * @param node AST node to validate
 * @return bool true if valid
 */
bool		ft_validate_logical_ast(t_ast_node *node);

/**
 * @brief Counts logical operators in token list
 * 
 * @param token_h Head of token list
 * @return int Number of logical operators
 */
int			ft_count_logical_operators(t_list *token_h);

/**
 * @brief Checks if token list has logical operators
 * 
 * @param token_h Head of token list
 * @return bool true if logical operators present
 */
bool		ft_has_logical_operators(t_list *token_h);

/**
 * @brief Parse une expression avec pipe
 * 
 * @param tokens Liste de tokens
 * @param shell Structure shell
 * @return t_ast_node* Nœud AST pipe
 */
t_ast_node	*ft_parse_pipe_expression(t_list *tokens, t_shell *shell);

#endif
