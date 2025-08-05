/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/02 12:40:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARENTHESES_H
# define PARENTHESES_H

# include "../minishell.h"

/* *************************** PARENTHESES PARSING *************************** */

t_ast_node	*ft_parentheses(t_shell *shell, t_list *token_h, t_list *start, 
				t_list *end);
t_ast_node	*ft_parser_phase8(t_list *tokens, t_shell *shell);
t_ast_node	*ft_parse_parentheses_expression(t_shell *shell, t_list *tokens);
int			ft_parse_parentheses_groups(t_shell *shell);

/* ***************************** DETECTION FUNCTIONS ***************************** */

bool		ft_has_parentheses(t_list *token_h);
int			ft_count_parentheses(t_list *token_h);
bool		ft_is_open_paren(t_token *token);
bool		ft_is_close_paren(t_token *token);
void		ft_print_paren_detection_info(t_list *token_h);

/* ***************************** FIND FUNCTIONS ***************************** */

t_list		*ft_find_matching_paren(t_list *start);
t_list		*ft_find_first_open_paren(t_list *token_h);
t_list		*ft_find_outermost_paren_pair(t_list *token_h, t_list **end);
bool		ft_is_fully_parenthesized(t_list *start, t_list *end);

/* ************************** VALIDATION FUNCTIONS ************************** */

int			ft_validate_parentheses_balance(t_list *token_h);
bool		ft_validate_parentheses_content(t_list *start, t_list *end);
bool		ft_validate_paren_syntax(t_list *token_h);
int			ft_get_paren_depth(t_list *token_h, t_list *target);

/* **************************** UTILITY FUNCTIONS **************************** */

void		ft_print_parentheses_ast(t_ast_node *node, int level);
char		*ft_get_paren_info_str(t_list *token_h);
bool		ft_validate_parentheses_ast(t_ast_node *node);
int			ft_get_max_paren_depth(t_list *token_h);
void		ft_debug_paren_structure(t_list *token_h);

t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end);

#endif