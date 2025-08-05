/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/02 12:40:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/**
 * @brief Main lexical analysis entry point
 * 
 * @param shell Shell structure
 * @return int 0 on success, -1 on error
 */
int	ft_lexer_main(t_shell *shell)
{
	if (!shell || !shell->line)
		return (-1);
	return (ft_lexing_simple(shell));
}
