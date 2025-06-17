/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 16:25:29 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static int	ft_validate_basic_syntax(t_list *tokens)
{
	t_list	*curr;
	bool	expect_operand;
	bool	found_operand;

	if (!tokens)
		return (-1);
	curr = tokens;
	expect_operand = true;
	found_operand = false;
	while (curr)
	{
		if (ft_is_operator_token(curr->content.token))
		{
			if (expect_operand || !curr->next)
				return (-1);
			expect_operand = true;
		}
		else if (ft_is_word_token(curr->content.token))
		{
			expect_operand = false;
			found_operand = true;
		}
		curr = curr->next;
	}
	if (!found_operand)
		return (-1);
	return (0);
}

static int	ft_preprocess_tokens(t_shell *shell)
{
	if (ft_validate_basic_syntax(shell->token) < 0)
	{
		ft_printerr("minishell: syntax error\n");
		return (-1);
	}
	if (!ft_validate_parentheses(shell->token))
	{
		ft_printerr("minishell: unmatched parentheses\n");
		return (-1);
	}
	return (0);
}

static t_ast_node	*ft_build_ast(t_shell *shell)
{
	t_ast_node	*ast;

	ast = ft_parse_expression(shell, shell->token, shell->token, NULL);
	return (ast);
}

int	ft_parse(t_shell *shell)
{
	if (!shell || !shell->token)
	{
		ft_printerr("FT_PARSE0\n");
		return (-1);
	}
	if (ft_preprocess_tokens(shell) < 0)
	{
		ft_printerr("FT_PARSE1\n");
		return (-1);
	}
	shell->ast = ft_build_ast(shell);
	if (!shell->ast)
	{
		ft_printerr("FT_PARSE2\n");
		return (-1);
	}
	ft_print_ast_tree(shell->ast, 0);
	return (0);
}