/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:06:07 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

int	ft_add_token(t_list **tokens, char *value, t_token_type type)
{
	t_content	content;
	t_token		*token;

	token = ft_create_token(value, type);
	if (!token)
		return (-1);
	content.token = token;
	ft_lstadd_back(tokens, content, TYPE_TOKEN);
	return (0);
}

static int	ft_process_operator(t_list **tokens, char *line, int *i)
{
	char	*op;

	op = ft_extract_operator(line, i);
	if (ft_add_token(tokens, op, ft_get_operator_type(op)) == -1)
		return (-1);
	return (0);
}

static int	ft_process_word(t_list **tokens, char *line, int *i)
{
	char	*word;

	word = ft_extract_word(line, i);
	if (ft_add_token(tokens, word, TOKEN_WORD) == -1)
		return (-1);
	return (0);
}

t_list	*ft_lexing_simple(t_shell *shell)
{
	t_list	*tokens;
	char	*line;
	int		i;

	if (!shell || !shell->current_line)
		return (NULL);
	tokens = NULL;
	line = shell->current_line;
	i = 0;
	while (line[i])
	{
		ft_skip_spaces(line, &i);
		if (!line[i])
			break ;
		if (ft_is_operator(line[i]))
		{
			if (ft_process_operator(&tokens, line, &i) == -1)
				return (ft_lstfree_t(tokens));
		}
		else
			if (ft_process_word(&tokens, line, &i) == -1)
				return (ft_lstfree_t(tokens));
	}
	return (tokens);
}
