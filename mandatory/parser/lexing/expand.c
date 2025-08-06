/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:11:31 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

char	*ft_rm_quotes(char *str)
{
	int		len;
	char	*result;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

void	ft_exp_tokens(t_list *tokens, char **envp, t_shell *shell)
{
	t_list	*current;
	t_token	*token;
	char	*expanded;
	char	*cleaned;

	if (!envp)
		return ;
	current = tokens;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (token->type == TOKEN_WORD)
			{
				expanded = ft_exp_string(token->value, envp, shell);
				free(token->value);
				cleaned = ft_rm_quotes(expanded);
				free(expanded);
				token->value = cleaned;
			}
		}
		current = current->next;
	}
}

t_list	*ft_lex_new(t_shell *shell)
{
	t_list	*tokens;

	tokens = ft_lex_simple(shell);
	if (!tokens)
		return (NULL);
	ft_exp_tokens(tokens, shell->env->env_vars, shell);
	return (tokens);
}
