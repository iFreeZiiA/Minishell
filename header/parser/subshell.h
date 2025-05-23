/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/23 23:03:50 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SUBSHELL_H
# define SUBSHELL_H

# include "../minishell.h"
# include <sys/wait.h>

/* ---------------------------- PARENTHESES PARSING ---------------------------- */

t_list		*ft_find_matching_paren(t_list *start);
bool		ft_validate_parentheses(t_list *token_h);
t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end);
t_ast_node	*ft_parse_expression_with_parens(t_shell *shell, t_list *token_h,
                                             t_list *start, t_list *end);
int			ft_parsing_with_parentheses(t_shell *shell);
void		ft_print_group_ast(t_ast_node *node, int level);

/* ---------------------------- SUBSHELL EXECUTION ---------------------------- */

int			ft_execute_subshell(t_ast_node *content, t_env *parent_env);
int			ft_execute_group(t_ast_node *node, t_env *env);
int			ft_execute_logical_enhanced(t_ast_node *node, t_env *env);
int			ft_execute_single_command(t_ast_node *node, t_env *env);
int			ft_execute_pipe_from_ast(t_ast_node *node, t_env *env);
int			ft_execute_ast_enhanced(t_shell *shell);

#endif
