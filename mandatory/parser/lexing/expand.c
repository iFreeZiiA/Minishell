/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/27 18:48:19 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

char	*ft_remove_quotes(char *str)
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

void	ft_expand_tokens(t_list *tokens, char **envp, t_shell *shell)
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
				expanded = ft_expand_string(token->value, envp, shell);
				free(token->value);
				cleaned = ft_remove_quotes(expanded);
				free(expanded);
				token->value = cleaned;
			}
		}
		current = current->next;
	}
}

t_list	*ft_lexing_new(t_shell *shell)
{
	t_list	*tokens;

	tokens = ft_lexing_simple(shell);
	if (!tokens)
		return (NULL);
	ft_expand_tokens(tokens, shell->env->env_vars, shell);
	return (tokens);
}
