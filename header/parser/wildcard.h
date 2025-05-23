/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/23 23:03:53 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "../minishell.h"
# include <dirent.h>
# include <sys/stat.h>

int		ft_expand_wildcard(t_shell *shell, t_list **tok_h);
void	ft_exp_wildcard(t_shell *shell, t_list **tok_h);

#endif
