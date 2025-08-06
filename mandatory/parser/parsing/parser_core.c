/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:06:43 by jjorda           ###   ########.fr       */
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
	(void)tokens;
	return (0);
}

void	free_command_list(t_list *cmd_list)
{
	(void)cmd_list;
}
