/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 00:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 08:48:31 by jjorda           ###   ########.fr       */
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

/* ************************** VALIDATION FUNCTIONS ************************** */

int			ft_validate_parentheses_balance(t_list *token_h);
bool		ft_validate_parentheses_content(t_list *start, t_list *end);

t_ast_node	*ft_parse_group(t_shell *shell, t_list *start, t_list *end);

#endif