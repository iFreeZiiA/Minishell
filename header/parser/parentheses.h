/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/12 10:27:41 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARENTHESES_H
# define PARENTHESES_H

# include "../minishell.h"

/* *************************** PARENTHESES PARSING *************************** */

t_ast_node	*ft_parentheses(t_shell *shell, t_list *token_h, t_list *start, 
				t_list *end);

/* ***************************** FIND FUNCTIONS ***************************** */

t_list		*ft_find_matching_paren(t_list *start);
t_list		*ft_find_matching_open_paren(t_list *start, t_list *end);
t_list		*ft_skip_parentheses_group(t_list *start);
int			ft_get_parentheses_depth(t_list *token_h, t_list *target);

/* ************************** VALIDATION FUNCTIONS ************************** */

bool		ft_validate_parentheses(t_list *token_h);
int			ft_validate_parentheses_balance(t_list *token_h);
bool		ft_validate_parentheses_order(t_list *start, t_list *end);
bool		ft_validate_parentheses_content(t_list *start, t_list *end);
bool		ft_validate_parentheses_wrapped(t_list *start, t_list *end);

/* **************************** CHECK FUNCTIONS **************************** */

bool		ft_check_parentheses_syntax(t_list *token_h);

/* **************************** DEPTH FUNCTIONS **************************** */

int			ft_find_max_parentheses_depth(t_list *token_h);
bool		ft_is_token_at_depth_level(t_list *token_h, t_list *target, 
				int level);
t_list		*ft_find_token_at_depth(t_list *token_h, int level);
bool		ft_validate_parentheses_depth(t_list *token_h);

#endif