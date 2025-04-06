/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:24:16 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/06 14:28:29 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	*ft_lexing_err(t_shell *shell, char **vec)
{
	ft_lstfree(shell->token);
	if (vec)
		ft_free_vctr((void **) vec);
	return (NULL);
}