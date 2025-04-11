/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/11 17:46:45 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static t_list	*ft_new_token(char *s, t_list **tok_h, int *i, int *status, bool *quote)
{
	t_content	tok;
	int			start;

	*status = 0;
	// if (!(*tok_h))
	// return (ft_tok_err(status, -3));
	// tok = (*tok_h)->content;
	// if (!tok)
	// 	return (ft_tok_err(status, -1));
	if (*quote)
	{
		(*i)++;
		*quote = false;
	}
	start = *i;
	tok.token = (t_token *) (malloc(sizeof(t_token)));
	if (!tok.token)
		return (ft_tok_err(status, -1));
	tok.token->type = ft_get_type(s, i, quote);
	// ft_printerr("i: %d, start: %d\n", *i, start);
	if (tok.token->type == TOKEN_ERROR)
		return (ft_tok_err(status, -2));
	if (tok.token->type == TOKEN_DQUOTE || tok.token->type == TOKEN_QUOTE)
		start++;
	tok.token->value = ft_substr(s, start, *i - start);
	if (!tok.token->value)
		return (ft_tok_err(status, -1));
	if (!ft_lstadd_back(tok_h, tok, TYPE_TOKEN))
		return (ft_tok_err(status, -1));
	return (*tok_h);
}

// bool	ft_check_quotes(t_shell *shell)
// {
	
// }

int	ft_lexing(t_shell *shell)
{
	t_list	*tok_h;
	char	*line;
	bool	quote;
	int		i;
	int		status;

	tok_h = NULL;
	line = shell->current_line;
	quote = false;
	i = 0;
	while (line[i])
	{
		// ft_printerr("c: %c, bool: %d\n", line[i], quote);
		ft_new_token(line, &tok_h, &i, &status, &quote);
		if (status)
			return (status);
	}
	// ft_lexer_checker(shell);
	shell->token = tok_h;
	return (0);
}
// int	main(int argc, char **argv)
// {
// 	t_shell	shell;
// 	int		res;

// 	(void) argc;
// 	shell.current_line = argv[1];
// 	res = ft_lexing(&shell);
// 	ft_print_list(shell.token);
// 	if (res)
// 	{
// 		if (res == -1)
// 			ft_printerr("MALLOC ERRRO\n");
// 		if (res == -2)
// 			ft_printerr("WRITE ERROR\n");
// 		if (res == -3)
// 			ft_printerr("INVALID ARGUMENT\n");
// 		ft_printerr("%d\n", res);
// 		return (res);
// 	}
// 	return (0);
// }