/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 15:08:01 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Main wildcard expansion function
 * 
 * @param shell The shell structure
 * @param tok_h Token list head
 * @return int 0 on success, -1 on error
 */
int	ft_expand_wildcard(t_shell *shell, t_list **tok_h)
{
	if (!shell || !tok_h || !*tok_h)
		return (-1);
	ft_exp_wildcard(shell, tok_h);
	return (0);
}

/**
 * @brief Processes wildcard expansion for all tokens
 * 
 * @param shell The shell structure
 * @param tok_h Token list head
 */
void	ft_exp_wildcard(t_shell *shell, t_list **tok_h)
{
	t_list	*curr;
	t_list	*next;
	t_token	*token;

	curr = *tok_h;
	while (curr)
	{
		next = curr->next;
		token = curr->content.token;
		if (ft_needs_wildcard_expansion(token))
			ft_replace_wildcard_token(tok_h, curr);
		curr = next;
	}
	(void)shell;
}

/**
 * @brief Checks if token needs wildcard expansion
 * 
 * @param token The token to check
 * @return bool true if expansion needed
 */
bool	ft_needs_wildcard_expansion(t_token *token)
{
	int	i;

	if (!token || !token->value)
		return (false);
	if (token->type != TOKEN_WORD)
		return (false);
	i = 0;
	while (token->value[i])
	{
		if (ft_is_wildcard_char(token->value[i]))
			return (true);
		i++;
	}
	return (false);
}

/**
 * @brief Checks if character is a wildcard
 * 
 * @param c Character to check
 * @return bool true if wildcard character
 */
bool	ft_is_wildcard_char(char c)
{
	if (c == '*')
		return (true);
	return (false);
}

/**
 * @brief Replaces wildcard token with matched files
 * 
 * @param tok_h Token list head
 * @param tok_curr Current token to replace
 * @return bool true on success
 */
bool	ft_replace_wildcard_token(t_list **tok_h, t_list *tok_curr)
{
	char	**matches;
	t_list	*new_tokens;
	t_token	*token;

	token = tok_curr->content.token;
	matches = ft_get_matching_files(token->value);
	if (!matches)
		return (false);
	new_tokens = ft_create_match_tokens(matches);
	if (!new_tokens)
		return (false);
	return (ft_connect_new_tokens(tok_h, tok_curr, new_tokens));
}
