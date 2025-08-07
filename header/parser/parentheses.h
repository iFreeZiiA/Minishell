/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARENTHESES_H
# define PARENTHESES_H

# include "../minishell.h"

/* *************************** PARENTHESES PARSING *************************** */

t_ast_node	*ft_parse_parentheses_expression(t_shell *shell, t_list *tokens);
t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end);
t_ast_node	*ft_parse_with_parentheses(t_shell *shell, t_list *tokens);
t_ast_node	*ft_handle_parentheses_in_expression(t_shell *shell, t_list *tokens);
t_ast_node	*ft_parse_logical_with_parens(t_shell *shell, t_list *start, t_list *end);

/* ***************************** DETECTION FUNCTIONS ***************************** */

int			ft_has_parentheses(t_list *token_list);

/* ***************************** FIND FUNCTIONS ***************************** */

t_list		*ft_find_matching_paren(t_list *start);
t_list		*ft_find_first_open_paren(t_list *token_list);
t_list		*ft_find_last_close_paren(t_list *token_list);

/* ************************** VALIDATION FUNCTIONS ************************** */

int			ft_validate_parentheses_balance(t_list *token_h);

/* ***************************** UTILITY FUNCTIONS ***************************** */

t_ast_node	*ft_create_operator_node(node_type op_type, t_ast_node *left, t_ast_node *right);

#endif