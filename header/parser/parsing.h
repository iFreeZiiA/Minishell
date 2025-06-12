/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 11:14:52 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

/* *************************** MAIN PARSING ENTRY *************************** */

int				ft_parse(t_shell *shell);

/* *************************** PARSING UTILITIES **************************** */

bool			ft_is_operator_token(t_token *token);
bool			ft_is_word_token(t_token *token);
t_list			*ft_find_operator_at_level(t_list *start, t_list *end);
bool			ft_is_redirect_token(t_token_type type);
t_ast_node		*ft_parse_expression(t_shell *shell, t_list *token_h,
					t_list *start, t_list *end);

/* ************************* EXPRESSION PARSING **************************** */

t_ast_node		*ft_parse_group(t_shell *shell, t_list *start, t_list *end);
t_ast_node		*ft_parse_command(t_list *start, t_list *end);
int				ft_parse_redirections(t_shell *shell, t_list *token_h,
					t_command *cmd);

/* ***************************** AST CREATION ***************************** */

t_ast_node		*ft_create_command_node(char **args, t_list *redirs);
t_ast_node		*ft_create_op_node(t_token_type type, t_ast_node *left,
					t_ast_node *right);
t_ast_node		*ft_create_group_node(t_ast_node *content);

/* ****************************** AST CLEANUP ****************************** */

void			ft_free_command(t_command *cmd);
void			ft_free_ast(t_ast_node *ast);

/* ****************************** AST DISPLAY ****************************** */

void			ft_print_ast_tree(t_ast_node *ast, int level);

#endif
