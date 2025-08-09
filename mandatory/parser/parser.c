/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:57:41 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/**
 * @brief Valide l'input avant traitement
 * 
 * @param input Ligne de commande
 * @param shell Structure shell
 * @return int 0 succès, -1 erreur
 */
static int	ft_validate_basic_input(char *input, t_shell *shell)
{
	if (!input || !shell)
		return (-1);
	if (ft_strlen(input) == 0)
		return (-1);
	return (0);
}

int	ft_parse_input(char *input, t_shell *shell)
{
	t_list	*tokens;
	int		result;

	if (ft_validate_basic_input(input, shell) != 0)
		return (EXIT_FAILURE);
	shell->current_line = input;
	result = ft_handle_lexing(shell, &tokens);
	if (result != EXIT_SUCCESS)
		return (result);
	result = ft_handle_syntax_validation(shell, tokens);
	if (result != EXIT_SUCCESS)
		return (result);
	result = ft_handle_ast_creation(shell, tokens);
	if (result != EXIT_SUCCESS)
		return (result);
	return (EXIT_SUCCESS);
}
