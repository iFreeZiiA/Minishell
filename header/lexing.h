/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:43:30 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/10 12:58:52 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

/* ---------------------------------- ERR ----------------------------------- */

void			ft_print_list(t_list *tok_h);
void			*ft_tok_err(int *p_status, int status);
void			*ft_lstfree_t(t_list *tok_h);
void			*ft_clean_node(t_list *tok_curr);

/* --------------------------------- UTILS ---------------------------------- */

t_token_type	ft_get_type(char *s, int *i);
int				ft_lexer_checker(t_shell *shell);

/* --------------------------------- LEXING --------------------------------- */

int				ft_lexing(t_shell *shell);

#endif