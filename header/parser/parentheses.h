/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 21:47:52 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARENTHESES_H
# define PARENTHESES_H

# include "../minishell.h"

/* *************************** PARENTHESES PARSING *************************** */

// t_ast_node	*ft_parse_parentheses_expression(t_shell *shell, t_list *tokens);
t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end);

/* ***************************** DETECTION FUNCTIONS ***************************** */

// bool		ft_has_parentheses(t_list *token_h);
// bool		ft_is_open_paren(t_token *token);
// bool		ft_is_close_paren(t_token *token);

/* ***************************** FIND FUNCTIONS ***************************** */

// t_list		*ft_find_matching_paren(t_list *start);
// t_list		*ft_find_first_open_paren(t_list *token_h);

/* ************************** VALIDATION FUNCTIONS ************************** */

int			ft_validate_parentheses_balance(t_list *token_h);

#endif