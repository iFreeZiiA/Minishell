/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/08 20:34:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATE_H
# define VALIDATE_H

# include "../minishell.h"

/* ******************************** CONSTANTS ********************* */

# define MAX_SYNTAX_ERRORS	32
# define MAX_ERROR_MESSAGE	256
# define MAX_VALIDATION_DEPTH	64

/* ******************************** STRUCTURES ******************** */

/**
 * @brief Error types for validation
 */
// typedef enum e_syntax_error_type {
// 	SYNTAX_ERROR_NONE = 0,
// 	SYNTAX_ERROR_UNCLOSED_QUOTE,
// 	SYNTAX_ERROR_UNEXPECTED_TOKEN,
// 	SYNTAX_ERROR_MISSING_COMMAND,
// 	SYNTAX_ERROR_INVALID_PIPE,
// 	SYNTAX_ERROR_INVALID_REDIRECT,
// 	SYNTAX_ERROR_UNBALANCED_PAREN,
// 	SYNTAX_ERROR_EMPTY_GROUP,
// 	SYNTAX_ERROR_INVALID_OPERATOR,
// 	SYNTAX_ERROR_INCOMPLETE_COMMAND,
// 	SYNTAX_ERROR_MAX
// }	t_syntax_error_type;

// /**
//  * @brief Syntax error structure
//  */
// typedef struct s_syntax_error {
// 	t_syntax_error_type		type;
// 	int						line;
// 	int						column;
// 	t_token_type			expected;
// 	t_token_type			found;
// 	char					message[MAX_ERROR_MESSAGE];
// 	struct s_syntax_error	*next;
// }	t_syntax_error;

// /**
//  * @brief Precedence levels for operators
//  */
// typedef enum e_precedence {
// 	PREC_LOWEST = 0,
// 	PREC_OR,
// 	PREC_AND,
// 	PREC_PIPE,
// 	PREC_PAREN,
// 	PREC_HIGHEST
// }	t_precedence;

// /**
//  * @brief Validation context structure
//  */
// typedef struct s_validation_context {
// 	t_list			*tokens;
// 	t_syntax_error	*errors;
// 	int				error_count;
// 	int				current_line;
// 	int				current_column;
// 	bool			in_group;
// 	int				paren_depth;
// }	t_validation_context;

/* *************************** VALIDATION FUNCTIONS ************************* */

/* validation.c */
int		ft_val_syntax(t_list *tokens);
// t_syntax_error		*ft_get_syntax_errors(t_list *tokens);
// void				ft_print_syntax_error(t_syntax_error *error);
// bool				ft_detect_syntax_error(char *command);

/* precedence.c */
// t_precedence		ft_get_precedence(t_token_type type);
// int					ft_get_command_precedence(char *command);
// t_ast_node			*ft_parse_with_precedence(t_list **tokens, 
// 						t_precedence min_prec);
// bool				ft_validate_precedence_order(t_list *tokens);

/* ast_validator.c */
// bool				ft_validate_ast_structure(t_ast_node *ast);
// int					ft_check_ast_consistency(t_ast_node *node);
// char				*ft_get_ast_error_message(int error_code);
// bool				ft_validate_ast_semantics(t_ast_node *ast);

/* utils/error_handler.c */
// t_syntax_error		*ft_create_syntax_error(t_syntax_error_type type,
// 						int line, int column);
// void				ft_add_syntax_error(t_validation_context *ctx,
// 						t_syntax_error *error);
// void				ft_free_syntax_errors(t_syntax_error *errors);
// char				*ft_get_error_type_string(t_syntax_error_type type);

/* *************************** VALIDATION CONTEXTS ************************** */

/* Context management */
// t_validation_context	*ft_create_validation_context(t_list *tokens);
// void					ft_free_validation_context(t_validation_context *ctx);
// void					ft_reset_validation_context(t_validation_context *ctx);

/* Specific validations */
int		ft_val_pipes(t_list *tokens);
int		ft_val_redir(t_list *tokens);
int		ft_val_paren_bal(t_list *tokens);
int		ft_val_ops(t_list *tokens);
int		ft_val_comp_cmds(t_list *tokens);
int		ft_val_heredoc_syn(t_list *tokens);
int		ft_val_comp_syntax(char *input, t_list *tokens);
int		ft_is_empty_ws(char *input);
void	ft_print_heredoc(t_token *next_token);
int		ft_check_paren(int *paren_count);

/* validate_substitutions.c */
int		ft_validate_substitutions(char *input);

/* utils.c */
int		ft_is_op_token(t_token *token);
int		ft_check_inv_tok_seq(t_token *prev, t_token *current);
int		ft_has_cmd_after_pipe(t_list *pipe_node);
int		ft_has_cmd_after_log(t_list *logical_node);

#endif
