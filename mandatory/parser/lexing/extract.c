/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 18:01:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static void	ft_skip_quotes(char *line, int *i, char quote_char)
{
	(*i)++;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (line[*i] == quote_char)
		(*i)++;
}

char	*ft_ext_word(char *line, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	while (line[*i] && !ft_is_space(line[*i]) && !ft_is_operator(line[*i]))
	{
		if (line[*i] == '"')
			ft_skip_quotes(line, i, '"');
		else if (line[*i] == '\'')
			ft_skip_quotes(line, i, '\'');
		else
			(*i)++;
	}
	len = *i - start;
	word = ft_substr(line, start, len);
	return (word);
}

char	*ft_ext_op(char *line, int *i)
{
	char	*op;
	int		start;

	start = *i;
	if (line[*i] == '|' && line[*i + 1] == '|')
		*i += 2;
	else if (line[*i] == '&' && line[*i + 1] == '&')
		*i += 2;
	else if (line[*i] == '>' && line[*i + 1] == '>')
		*i += 2;
	else if (line[*i] == '<' && line[*i + 1] == '<')
		*i += 2;
	else
		(*i)++;
	op = ft_substr(line, start, *i - start);
	return (op);
}

t_token_type	ft_get_op_type(char *op)
{
	if (!ft_strcmp(op, "|"))
		return (TOKEN_PIPE);
	if (!ft_strcmp(op, "||"))
		return (TOKEN_OR);
	if (!ft_strcmp(op, "&&"))
		return (TOKEN_AND);
	if (!ft_strcmp(op, ">"))
		return (TOKEN_REDIR_OUT);
	if (!ft_strcmp(op, "<"))
		return (TOKEN_REDIR_IN);
	if (!ft_strcmp(op, ">>"))
		return (TOKEN_APPEND);
	if (!ft_strcmp(op, "<<"))
		return (TOKEN_HEREDOC);
	if (!ft_strcmp(op, "("))
		return (TOKEN_PAREN_OPEN);
	if (!ft_strcmp(op, ")"))
		return (TOKEN_PAREN_CLOSE);
	return (TOKEN_WORD);
}
