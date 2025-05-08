/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:32 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/08 14:06:36 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Expands a variable token into its value
 * 
 * @param shell The shell structure
 * @param tok_h The token list head
 * @param tok_c Current token node
 * @param tok_n Next token node
 * @return t_list* Updated current token
 */
static t_list	*ft_expand_token(t_shell *shell, t_list **tok_h, 
                                t_list **tok_c, t_list **tok_n)
{
	t_token	*token;
	char	*env_value;
	t_token	*new_token;
	t_list	*new_node;

	if (!shell || !(*tok_h) || !(*tok_c) || !(*tok_c)->content.token)
		return (NULL);
	token = (*tok_c)->content.token;
	if (!(*tok_c)->next || (*tok_c)->next->content.token->type != TOKEN_WORD)
	{
		token->type = TOKEN_WORD;
		return (*tok_c);
	}
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	env_value = ft_getenv_value(shell, 
		(*tok_c)->next->content.token->value, 0, 
		ft_strlen((*tok_c)->next->content.token->value));
	if (!env_value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->value = env_value;
	new_token->type = TOKEN_WORD;
	new_node = ft_lstnew_tok(new_token);
	if (!new_node)
	{
		free(new_token->value);
		free(new_token);
		return (NULL);
	}
	if (!ft_lstreplace_n(tok_c, new_node, ft_clean_node_tok, 2))
	{
		ft_clean_node_tok(new_node);
		return (NULL);
	}
	if (!(*tok_c)->prev)
	// {
	// 	ft_printerr("EXPAND_TOKEN\n");
		*tok_h = *tok_c;
	// }
	*tok_n = (*tok_c)->next;
	return (*tok_c);
}

/**
 * @brief Expands the $? status variable to its value
 * 
 * @param shell The shell structure
 * @param tok_c Current token node
 * @param str The string containing the status variable
 * @return bool true on success, false on error
 */
bool	ft_expand_status(t_shell *shell, t_list *tok_c, char *str)
{
	char	*new_str;
	char	*status;
	int		i;
	int		j;
	int		status_len;
	int		str_len;
	
	if (!shell || !tok_c || !str)
		return (false);
	status = ft_itoa(shell->env->last_exit_code);
	if (!status)
		return (false);
	status_len = ft_strlen(status);
	str_len = ft_strlen(str);
	new_str = (char *)malloc(sizeof(char) * (str_len + status_len + 1));
	if (!new_str)
	{
		free(status);
		return (false);
	}
	i = 0;
	j = 0;
	while (i < str_len)
	{
		if (str[i] == DOLL && str[i + 1] == Q_MRK)
		{
			ft_strlcpy(new_str + j, status, status_len + 1);
			j += status_len;
			i += 2;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free(status);
	free(str);
	tok_c->content.token->value = new_str;
	return (true);
}

/**
 * @brief Expands a variable in a string to its value
 * 
 * @param shell The shell structure
 * @param tok_c Current token node
 * @param str The string containing the variable
 * @param var_pos Position of the variable in the string
 * @return bool true on success, false on error
 */
bool	ft_expand_var(t_shell *shell, t_list *tok_c, char *str, int var_pos)
{
	char	*new_str;
	char	*var_value;
	int		var_name_len;
	int		i;
	int		j;
	int		k;
	
	if (!shell || !tok_c || !str || var_pos < 0)
		return (false);
	var_name_len = ft_eov(&str[var_pos]);
	if (var_name_len <= 0)
		return (false);
	var_value = ft_getenv_value(shell, str, var_pos, var_name_len);
	if (!var_value)
		return (false);
	new_str = (char *)malloc(sizeof(char) * 
		(ft_strlen(str) - var_name_len - 1 + ft_strlen(var_value) + 1));
	if (!new_str)
	{
		free(var_value);
		return (false);
	}
	i = 0;
	j = 0;
	while (i < var_pos - 1)
		new_str[j++] = str[i++];
	i = var_pos + var_name_len;
	k = 0;
	while (var_value[k])
		new_str[j++] = var_value[k++];
	while (str[i])
		new_str[j++] = str[i++];
	new_str[j] = '\0';
	free(var_value);
	free(str);
	tok_c->content.token->value = new_str;
	return (true);
}

/**
 * @brief Processes expansions within a double-quoted string
 * 
 * @param shell The shell structure
 * @param tok_c Current token node
 * @param status Pointer to status variable
 * @return bool true on success, false on error
 */
static bool	ft_expand_dquote(t_shell *shell, t_list *tok_c, int *status)
{
	t_token	*token;
	int		i;
	
	if (!shell || !tok_c || !status)
		return (false);
	token = tok_c->content.token;
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == DOLL && token->value[i + 1] == Q_MRK)
		{
			if (!ft_expand_status(shell, tok_c, token->value))
			{
				*status = -1;
				return (false);
			}
			i = 0;
			continue;
		}
		else if (token->value[i] == DOLL && 
				(ft_isalpha(token->value[i + 1]) || token->value[i + 1] == '_'))
		{
			if (!ft_expand_var(shell, tok_c, token->value, i + 1))
			{
				*status = -1;
				return (false);
			}
			i = 0;
			continue;
		}
		i++;
	}
	return (true);
}

/**
 * @brief Main expansion function to process all token expansions
 * 
 * @param shell The shell structure
 * @param tok_h The token list head
 * @param status Pointer to status variable
 * @return int 0 on success, -1 on error
 */
int	ft_expansion(t_shell *shell, t_list **tok_h, int *status)
{
	t_list	*tok_c;
	t_list	*tok_n;
	t_token	*token;
	
	if (!shell || !(*tok_h) || !status)
		return (-1);
	tok_c = *tok_h;
	while (tok_c)
	{
		token = tok_c->content.token;
		tok_n = tok_c->next;
		if (token->type == TOKEN_STATUS)
		{
			free(token->value);
			token->value = ft_itoa(shell->env->last_exit_code);
			if (!token->value)
				return (-1);
			token->type = TOKEN_WORD;
		}
		else if (token->type == TOKEN_VAR)
		{
			// ft_printerr("EXPANSION, h: %p\n", (*tok_h)->content.token->value);
			tok_c = ft_expand_token(shell, tok_h, &tok_c, &tok_n);
			if (!tok_c)
				return (-1);
			// ft_printerr("EXPANSION, h: %p\n", (*tok_h)->content.token->value);
		}
		else if (token->type == TOKEN_DQUOTE)
		{
			if (!ft_expand_dquote(shell, tok_c, status))
				return (-1);
		}
		tok_c = tok_n;
	}
	return (0);
}
