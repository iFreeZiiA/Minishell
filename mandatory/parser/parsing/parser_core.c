/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:15:38 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

t_ast_node	*ft_parser(t_list *tokens, t_shell *shell)
{
	t_ast_node	*result;

	result = ft_enhanced_parser(tokens, shell);
	return (result);
}

int	ft_validate_token_list(t_list *tokens)
{
	int	validation_result;

	if (!tokens)
		return (EXIT_SYNTAX);
	validation_result = ft_val_syntax(tokens);
	if (validation_result != 0)
	{
		ft_printerr("minishell: syntax error\n");
		return (EXIT_SYNTAX);
	}
	return (EXIT_SUCCESS);
}

void	free_command_list(t_list *cmd_list)
{
	(void)cmd_list;
}
