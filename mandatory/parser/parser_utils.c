/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:00:00 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 16:57:41 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	ft_cleanup_parsing_error(t_shell *shell, t_list *tokens)
{
	if (tokens)
		ft_lstfree_t(tokens);
	if (shell && shell->ast)
	{
		ft_free_ast(shell->ast);
		shell->ast = NULL;
	}
}

int	ft_handle_lexing(t_shell *shell, t_list **tokens)
{
	*tokens = ft_lex_new(shell);
	if (!*tokens)
	{
		shell->ast = NULL;
		return (EXIT_FAILURE);
	}
	shell->token = *tokens;
	return (EXIT_SUCCESS);
}

int	ft_handle_syntax_validation(t_shell *shell, t_list *tokens)
{
	int	syntax_result;

	syntax_result = ft_val_comp_syntax(shell->current_line, shell->token);
	if (syntax_result != 0)
	{
		ft_cleanup_parsing_error(shell, tokens);
		return (EXIT_SYNTAX);
	}
	return (EXIT_SUCCESS);
}

int	ft_handle_ast_creation(t_shell *shell, t_list *tokens)
{
	shell->ast = ft_parser(shell->token, shell);
	if (!shell->ast)
	{
		ft_cleanup_parsing_error(shell, tokens);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_process_command_line(t_shell *shell)
{
	if (!shell || !shell->current_line)
		return (-1);
	return (ft_parse_input(shell->current_line, shell));
}
