/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 14:03:43 by jjorda           ###   ########.fr       */
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

static char	*ft_process_quoted_word(char *word)
{
	int		len;
	char	*temp;
	char	*result;
	char	*equals_pos;
	char	*before_quotes;
	char	*quoted_part;

	len = ft_strlen(word);
	
	// Cas 1: Le mot entier est entouré de quotes (cas simple)
	if (len >= 2)
	{
		if (word[0] == '\'' && word[len - 1] == '\'')
		{
			// Pour les single quotes, marquer avec \x01 et retirer les quotes
			temp = ft_substr(word, 1, len - 2);
			result = ft_strjoin("\x01", temp);
			free(temp);
			free(word);
			return (result);
		}
		else if (word[0] == '"' && word[len - 1] == '"')
		{
			// Pour les double quotes, juste retirer les quotes (expansion normale)
			result = ft_substr(word, 1, len - 2);
			free(word);
			return (result);
		}
	}
	
	// Cas 2: Le mot contient un = suivi de quotes (ex: VAR='value')
	equals_pos = ft_strchr(word, '=');
	if (equals_pos && equals_pos[1] == '\'' && word[len - 1] == '\'')
	{
		// Séparer la partie avant le = et la partie quotée
		before_quotes = ft_substr(word, 0, equals_pos - word + 1); // Inclut le '='
		quoted_part = ft_substr(word, equals_pos - word + 2, len - (equals_pos - word + 2) - 1); // Sans les quotes
		
		// Marquer la partie quotée avec \x01
		temp = ft_strjoin("\x01", quoted_part);
		result = ft_strjoin(before_quotes, temp);
		
		free(before_quotes);
		free(quoted_part);
		free(temp);
		free(word);
		return (result);
	}
	else if (equals_pos && equals_pos[1] == '"' && word[len - 1] == '"')
	{
		// Double quotes après = : retirer juste les quotes (expansion normale)
		before_quotes = ft_substr(word, 0, equals_pos - word + 1); // Inclut le '='
		quoted_part = ft_substr(word, equals_pos - word + 2, len - (equals_pos - word + 2) - 1); // Sans les quotes
		result = ft_strjoin(before_quotes, quoted_part);
		
		free(before_quotes);
		free(quoted_part);
		free(word);
		return (result);
	}
	
	// Retourner le mot inchangé si pas de quotes
	return (word);
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
	return (ft_process_quoted_word(word));
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
