/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:10 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/08 18:41:11 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static t_list	*ft_new_token(char *s, t_list **tok_h, int *i, int *status)
{
	t_token	*tok;
	int		start;

	*status = 0;
	tok = (t_token *) malloc(sizeof(t_token));
	if (!tok)
		return (ft_tok_err(status, -1));
	start = *i;
	tok->type = ft_get_type(s, i);
	if (tok->type == TOKEN_ERROR)
		return (ft_tok_err(status, -2));
	tok->value = ft_substr(s, start, *i - start);
	if (!tok->value)
		return (ft_tok_err(status, -1));
	if (!ft_lstadd_back(tok_h, (void *) tok))
		return (ft_tok_err(status, -1));
	return (*tok_h);
}

int	ft_lexing(t_shell *shell)
{
	t_list	*tok_h;
	char	*line;
	int		i;
	int		status;

	tok_h = NULL;
	line = shell->current_line;
	i = 0;
	while (line[i])
	{
		ft_new_token(line, &tok_h, &i, &status);
		if (status)
			return (status);
	}
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
// 		ft_printerr("%d\n", res);
// 		return (res);
// 	}
// 	return (0);
// }