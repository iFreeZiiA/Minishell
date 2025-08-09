/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:25:52 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:55:51 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "../minishell.h"

int			ft_validate_complete_syntax(char *input, t_list *tokens);
int			ft_validate_quotes_in_input(char *input);
int			ft_validate_parentheses_balance(t_list *tokens);
int			ft_validate_compound_commands(t_list *tokens);
int			ft_validate_heredoc_syntax(t_list *tokens);
int			ft_is_operator_token(t_token *token);
int			ft_check_invalid_token_sequence(t_token *prev, t_token *current);
int			ft_has_command_after_pipe(t_list *pipe_node);
int			ft_has_command_after_logical(t_list *logical_node);
int			ft_is_empty_or_whitespace(char *input);
void		ft_print_syntax_error_bash(t_token *token);
char		*ft_get_token_name_for_error(t_token_type type);

/**
 * @brief Parses a group (parentheses)
 * 
 * @param shell Shell structure
 * @param start Opening parenthesis token
 * @param end Closing parenthesis token
 * @return t_ast_node* Group AST node
 */
// t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end);

/**
 * @brief Expands wildcards in token list
 * 
 * @param shell Shell structure
 * @param tok_h Token list head pointer
 */
// void	ft_exp_wildcard(t_shell *shell, t_list **tok_h);

/* ----------------------------- AST UTILITIES ----------------------------- */

/**
 * @brief Creates a command node in the AST
 * 
 * @param args Array of command arguments
 * @param redirs List of redirections
 * @return t_ast_node* New command node, NULL on error
 */
t_ast_node	*ft_new_cmd_node(char **args, t_list *redirs);

/**
 * @brief Creates an operator node in the AST
 * 
 * @param type Type of operator (PIPE, AND, OR)
 * @param left Left child node
 * @param right Right child node
 * @return t_ast_node* New operator node, NULL on error
 */
// t_ast_node	*ft_create_operator_node(t_node_type type, t_ast_node *left,
//			t_ast_node *right);

/**
 * @brief Creates a group node in the AST
 * 
 * @param content Content of the group
 * @return t_ast_node* New group node, NULL on error
 */
t_ast_node	*ft_new_grp_node(t_ast_node *content);

/**
 * @brief Parses redirections from tokens and adds them to a command
 * 
 * @param shell The shell structure
 * @param token_h Head of the token list
 * @param cmd Command structure to add redirections to
 * @return int 0 on success, -1 on error
 */
int			ft_parse_redir(t_shell *shell, t_list *token_h,
				t_command *cmd);

#endif