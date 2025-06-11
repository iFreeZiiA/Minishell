/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/06/11 11:49:06 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

/* ============================= MAIN PARSING ============================= */

/**
 * @brief Main parsing function to build the AST
 * 
 * @param shell The shell structure
 * @return int 0 on success, -1 on error
 */
int				ft_parsing(t_shell *shell);

/* ============================ AST CREATION ============================ */

/**
 * @brief Creates a new AST node
 * 
 * @param type Node type
 * @param data Associated data
 * @param left Left child
 * @param right Right child
 * @return t_ast_node* New node or NULL on error
 */
t_ast_node		*ft_create_ast_node(node_type type, void *data,
					t_ast_node *left, t_ast_node *right);

/**
 * @brief Creates a command AST node
 * 
 * @param cmd Command structure
 * @return t_ast_node* New command node or NULL on error
 */
t_ast_node		*ft_create_command_ast(t_command *cmd);

/**
 * @brief Creates an operator AST node
 * 
 * @param op_type Operator type
 * @param left Left operand
 * @param right Right operand
 * @return t_ast_node* New operator node or NULL on error
 */
t_ast_node		*ft_create_operator_ast(node_type op_type,
					t_ast_node *left, t_ast_node *right);

/**
 * @brief Creates a group AST node
 * 
 * @param content Group content
 * @return t_ast_node* New group node or NULL on error
 */
t_ast_node		*ft_create_group_ast(t_ast_node *content);

/**
 * @brief Cleans up failed AST creation
 * 
 * @param left Left node to free
 * @param right Right node to free
 */
void			ft_cleanup_ast_creation(t_ast_node *left, t_ast_node *right);

/* ========================== EXPRESSION PARSING ========================== */

/**
 * @brief Parses expressions with operator precedence
 * 
 * @param shell Shell structure
 * @param token_h Token list head
 * @param start Expression start
 * @param end Expression end
 * @return t_ast_node* Expression AST node or NULL on error
 */
t_ast_node		*ft_parse_expression(t_shell *shell, t_list *token_h,
					t_list *start, t_list *end);

/* ============================ OPERATOR UTILS ============================ */

/**
 * @brief Finds operator token within expression bounds
 * 
 * @param start Expression start
 * @param end Expression end
 * @param target_type Target operator type
 * @return t_list* Operator token or NULL if not found
 */
t_list			*ft_find_operator(t_list *start, t_list *end,
					t_token_type target_type);

/**
 * @brief Converts token type to AST node type
 * 
 * @param token_type Token type
 * @return node_type Corresponding AST node type
 */
node_type		ft_token_to_node_type(t_token_type token_type);

/**
 * @brief Finds matching closing parenthesis
 * 
 * @param start Opening parenthesis token
 * @return t_list* Closing parenthesis token or NULL if not found
 */
t_list			*ft_find_matching_paren(t_list *start);

/**
 * @brief Checks if token is a logical operator
 * 
 * @param token Token to check
 * @return int 1 if logical operator, 0 otherwise
 */
int				ft_is_logical_operator(t_token *token);

/**
 * @brief Gets operator precedence level
 * 
 * @param type Token type
 * @return int Precedence level (higher = more precedent)
 */
int				ft_get_operator_precedence(t_token_type type);

/* =========================== COMMAND PARSING =========================== */

/**
 * @brief Parses simple command from tokens
 * 
 * @param shell Shell structure
 * @param start First token
 * @param end Last token boundary
 * @return t_ast_node* Command AST node or NULL on error
 */
t_ast_node		*ft_parse_simple_command(t_shell *shell,
					t_list *start, t_list *end);

/**
 * @brief Parses group (parenthesized) expression
 * 
 * @param shell Shell structure
 * @param start Opening parenthesis
 * @param end Closing parenthesis
 * @return t_ast_node* Group AST node or NULL on error
 */
t_ast_node		*ft_parse_group(t_shell *shell, t_list *start, t_list *end);

/* ============================ PARSING UTILS ============================ */

/**
 * @brief Frees arguments array
 * 
 * @param args Arguments array
 * @param count Number of allocated elements
 * @return char** NULL
 */
char			**ft_free_args_array(char **args, int count);

/**
 * @brief Frees command structure
 * 
 * @param cmd Command to free
 */
void			ft_free_command_struct(t_command *cmd);

/**
 * @brief Validates parentheses balance in token list
 * 
 * @param token_list Token list to validate
 * @return int 1 if balanced, 0 otherwise
 */
int				ft_validate_parentheses(t_list *token_list);

/**
 * @brief Checks if token is a redirection operator
 * 
 * @param token Token to check
 * @return int 1 if redirection, 0 otherwise
 */
int				ft_is_redirection_token(t_token *token);

/**
 * @brief Counts tokens of specific type in range
 * 
 * @param start Start token
 * @param end End boundary
 * @param type Token type to count
 * @return int Count of matching tokens
 */
int				ft_count_tokens_of_type(t_list *start, t_list *end,
					t_token_type type);

/* ============================= AST DISPLAY ============================= */

/**
 * @brief Prints token list for debugging
 * 
 * @param token_list Token list to print
 */
void			ft_print_token_list(t_list *token_list);

/**
 * @brief Prints AST structure recursively
 * 
 * @param ast AST node to print
 * @param level Current indentation level
 */
void			ft_print_ast(t_ast_node *ast, int level);

#endif
