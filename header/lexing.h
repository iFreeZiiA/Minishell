/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:43:30 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/06 14:28:41 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H


int		ft_lexing(t_shell *shell);
char	**ft_lexing_split(char *str);
void	*ft_lexing_err(t_shell *shell, char **vec);

#endif