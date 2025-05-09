/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/09 16:58:04 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Creates a new token and adds it to the token list
 * 
 * @param s The input string
 * @param tok_h Pointer to the head of the token list
 * @param i Current position in the string
 * @param quote Flag indicating if we're inside quotes
 * @return int 0 on success, -1 on memory allocation failure, -2 on parsing
 * error
 */
static int	ft_new_token(char *s, t_list **tok_h, int *i, bool *quote)
{
	t_content	tok;
	int			start;
	int			end;

	if (*quote)
	{
		(*i)++;
		*quote = false;
	}
	start = *i;
	tok.token = (t_token *) malloc(sizeof(t_token));
	if (!tok.token)
		return (-1);
	// ft_printerr("new_token: %d, %s\n", *i, ft_gettype_name(tok.token->type));
	tok.token->type = ft_get_type(s, i, quote);
	// ft_printerr("new_token: %d, %s\n", *i, ft_gettype_name(tok.token->type));
	if (tok.token->type == TOKEN_ERROR)
		return (ft_new_token_err(tok.token, -2));
	if (tok.token->type == TOKEN_DQUOTE || tok.token->type == TOKEN_QUOTE)
		start++;
	end = *i - start;
	// ft_printerr("end: %d, i: %d, start: %d\n", end, *i, start);
	tok.token->value = ft_substr(s, start, end);
	if (!tok.token->value)
		return (ft_new_token_err(tok.token, -1));
	if (!ft_lstadd_back(tok_h, tok, TYPE_TOKEN))
	{
		free(tok.token->value);
		return (ft_new_token_err(tok.token, -1));
	}
	return (0);
}

/**
 * @brief Main lexing function to tokenize a string
 * 
 * @param shell The shell structure
 * @return t_list* Head of the token list, NULL on error
 */
t_list	*ft_lexing(t_shell *shell)
{
	t_list	*tok_h;
	bool	quote;
	int		status;
	int		i;

	if (!shell || !shell->current_line)
		return (NULL);
	tok_h = NULL;
	quote = false;
	i = 0;
	while (shell->current_line[i])
	{
		// ft_printerr("LEXING, %c\n", shell->current_line[i]);
		status = ft_new_token(shell->current_line, &tok_h, &i, &quote);
		if (status < 0)
			return (ft_lstfree_t(tok_h));
	}
	status = 0;
	if (ft_expansion(shell, &tok_h, &status) < 0)
		return (ft_lstfree_t(tok_h));
	shell->token = tok_h;
	return (tok_h);
}
