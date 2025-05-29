/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/29 18:17:47 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGICAL_H
# define LOGICAL_H

# include "../minishell.h"

/* ----------------------------- LOGICAL EXECUTION ----------------------------- */

int		ft_execute_logical(t_ast_node *node, t_env *env);
int		ft_execute_ast(t_shell *shell);
bool	ft_is_builtin_cmd(const char *cmd);
void	ft_print_logical_ast(t_ast_node *node, int level);
char	*ft_get_logical_op_str(node_type type);

/* --------------------------- LOGICAL EXECUTION UTILS ------------------------- */

int		ft_handle_or_operator(t_ast_node *node, t_env *env, int left_exit_code);
int		ft_execute_node(t_ast_node *node, t_env *env);
int		ft_execute_pipe_node(t_ast_node *node, t_env *env);
void	ft_lstfree_cmd_list(t_list *cmd_list);
void	ft_free_command_from_list(t_command *cmd);

/* --------------------------- LOGICAL PRINT UTILS ---------------------------- */

void	ft_print_operator_node(t_ast_node *node, int level);
void	ft_print_command_node(t_ast_node *node);
void	ft_print_group_node(t_ast_node *node, int level);

/* ----------------------------- LOGICAL PARSING ----------------------------- */

t_ast_node	*ft_parse_expression(t_shell *shell, t_list *token_h, 
                                  t_list *start, t_list *end);
t_ast_node	*ft_parse_simple_command(t_shell *shell, t_list *start, t_list *end);
int			ft_parsing_with_logical(t_shell *shell);

#endif
