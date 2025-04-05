/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:47:46 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/05 20:07:30 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

token_type	ft_get_token_type(char *token)
{
	if (token[0] == '|' && !token[1])
		return (TOKEN_PIPE);
	else if (token[0] == '|' && token[1] == '|' && !token[2])
		return (TOKEN_OR);
	else if (token[0] == '&' && token[1] == '&' && !token[2])
		return (TOKEN_AND);
	else if (token[0] == '>' && !token[1])
		return (TOKEN_REDIR_OUT);
	else if (token[0] == '>' && token[1] == '>' && !token[2])
		return (TOKEN_APPEND);
	else if (token[0] == '<' && !token[1])
		return (TOKEN_REDIR_IN);
	else if (token[0] == '<' && token[1] == '<' && !token[2])
		return (TOKEN_HEREDOC);
	else if (token[0] == '(' && !token[1])
		return (TOKEN_PAREN_OPEN);
	else if (token[0] == ')' && !token[1])
		return (TOKEN_PAREN_CLOSE);
	else if (token[0] == '$' && !token[1])
		return (TOKEN_DOLLAR);
	else if (token[0] == '*' && !token[1])
		return (TOKEN_WILDCARD);
	else if (!token[1])
		return (TOKEN_EOF);
	return (TOKEN_WORD);
}

t_token	*ft_new_token(char *str)
{
	t_token	*token;

	token = (t_token *) malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = ft_get_token_type(str);
	token->value = str;
	return (token);
}

t_list	*ft_get_token(int argc, char **argv)
{
	t_list		*tokens;
	t_token		*token;
	int			i;
	int			j;

	if (argc == 0)
		return (NULL);
	i = 0;
	while(++i < argc)
	{
		j = 0;
		ft_printerr("Ping ft_get_token 0\n");
		while(ft_isspace(argv[i][j]))
			j++;
		ft_printerr("Ping ft_get_token 1\n");
		token = ft_new_token(argv[i]);
		if (!token)
			return (NULL);
		ft_printerr("Ping ft_get_token 2\n");
		ft_lstadd_back(&tokens, (void *) token);
		if (!tokens->next && !ft_lstlast(tokens))
			return (NULL);
		ft_printerr("Ping ft_get_token 3\n");
	}
	ft_printerr("Ping ft_get_token 4\n");
	return (tokens);
}

int	main(int argc, char **argv)
{
	t_list	*tokens;
	t_list	*next;

	(void) argc;
	ft_printerr("Ping main 0\n");
	tokens = ft_get_token(argc, argv);
	ft_printerr("Ping main 1\n");
	next = tokens;
	while ((next = next->next))
		ft_printerr("%s\n", (char *) next->content);
	return (0);
}

// void	ft_delete_token(t_list *token_list)
// {
	
// }
