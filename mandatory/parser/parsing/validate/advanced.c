/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:55:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_check_final_bal(int paren_count)
{
	if (paren_count > 0)
	{
		ft_printerr("bash: syntax error: unexpected end of file\n");
		return (2);
	}
	return (0);
}

int	ft_val_paren_bal(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		paren_count;
	int		result;

	current = tokens;
	paren_count = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_PAREN_OPEN)
			paren_count++;
		else if (token->type == TOKEN_PAREN_CLOSE)
		{
			result = ft_check_paren(&paren_count);
			if (result != 0)
				return (result);
		}
		current = current->next;
	}
	return (ft_check_final_bal(paren_count));
}

int	ft_val_comp_cmds(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		word_count;

	current = tokens;
	word_count = 0;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_WORD)
			word_count++;
		else if (ft_is_op_token(token))
		{
			if (word_count == 0)
			{
				ft_print_syntax_error_bash(token);
				return (2);
			}
			word_count = 0;
		}
		current = current->next;
	}
	return (0);
}

static int	ft_val_heredoc_delim(t_list *current)
{
	t_token	*next_token;

	if (!current->next)
	{
		ft_printerr("bash: syntax error near unexpected token `newline'\n");
		return (2);
	}
	next_token = (t_token *)current->next->content.token;
	if (!next_token || next_token->type != TOKEN_WORD)
	{
		ft_print_heredoc(next_token);
		return (2);
	}
	return (0);
}

int	ft_val_heredoc_syn(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		result;

	current = tokens;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_HEREDOC)
		{
			result = ft_val_heredoc_delim(current);
			if (result != 0)
				return (result);
		}
		current = current->next;
	}
	return (0);
}
