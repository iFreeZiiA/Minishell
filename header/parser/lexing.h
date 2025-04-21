/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:43:30 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/21 18:24:16 by jjorda           ###   ########.fr       */
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

t_token_type	ft_get_type(char *s, int *i, bool *quote);
int				ft_lexer_checker(t_shell *shell);

/* --------------------------------- LEXING --------------------------------- */

t_list			*ft_lexing(t_shell *shell, char *prompt, int *status);

int				ft_expansion(t_shell *shell, t_list *tok_h, int *status);

#endif