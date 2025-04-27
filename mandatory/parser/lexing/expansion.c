/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:05:32 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/27 17:22:03 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

t_list	*ft_getexpan(t_shell *shell, t_list *tok_h, t_list **tok_c, t_token *tok_n)
{
	t_list	*tok_l;
	t_token	*new_tok;

	if (!shell || !tok_h || !(*tok_c)->content.token || !tok_n)
		return (NULL);
	// ft_printerr("PING\n");
	if (!(*tok_c)->next || (*tok_c)->next->content.token->type != TOKEN_WORD)
	{
		(*tok_c)->content.token->type = TOKEN_WORD;
		return (*tok_c);
	}
	new_tok = malloc(sizeof(t_token));
	if (!new_tok)
		return (NULL);
	// ft_printerr("Ping getexpan 0\n");
	new_tok->value = ft_getenv_value(shell, (*tok_c)->next->content.token->value);
	// ft_printerr("Ping getexpan 1\n");
	new_tok->type = TOKEN_WORD;
	tok_l = ft_lstnew_tok(new_tok);
	// ft_printerr("Ping getexpan 2\n");
	if (!ft_lstreplace_n(tok_c, tok_l, ft_clean_node_tok, 2))
		return (NULL);
	// ft_printerr("Ping getexpan 3\n");
	return (tok_l);
}

char	*ft_expand(char *str, int *i, char *to_rep)
{
	
}

bool	ft_expan_dquote(t_shell *shell, t_list *tok_c, int *status)
{
	t_token	*tok;
	char	*str;
	char	*new_str;
	char	*var;
	int		i;

	if (!shell || !tok_c)
		return (false);
	tok = tok_c->content.token;
	str = tok->value;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			new_str = ft_expand(tok->value, &i, ft_itoa(shell->env->last_exit_code));
		else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i] == '_'))
		{
			new_str = ft_substr(str, i, ft_eov(&str[i]));
			
			var = ft_getenv_value(shell, str, i, ft_eov(&str[i]));
			free(new_str);
			if (!var)
				return (false);
			new_str = ft_expand(tok->value, &i, var);
			free(var);
			if (!new_str)
				return (false);
			
		}
	}
	return (true);
}

int	ft_expansion(t_shell *shell, t_list *tok_h, int *status)
{
	t_list	*tok_c;
	t_list	*tok_n;
	t_token	*tok;
	bool	head;

	if (!shell || !tok_h)
		return (-1);
	tok_c = tok_h;
	head = false;
	while (tok_c)
	{
		tok = tok_c->content.token;
		tok_n = tok_c->next;
		// ft_printerr("Ping ft_expansion 0: %s, %d\n", tok->value, tok->type);
		if (tok->type == TOKEN_STATUS)
		{
			// ft_printerr("Ping ft_expansion 1: %s, %d\n", tok->value, tok->type);
			free(tok->value);
			tok->value = ft_itoa(shell->env->last_exit_code);
		}
		else if (tok->type == TOKEN_VAR)
		{
			if (tok_h == tok_c)
				head = true;
			// ft_printerr("Ping ft_expansion 2: %s, %d\n", tok->value, tok->type);
			if (!tok_c->next)
				tok->type = TOKEN_WORD;
			else
			{
				tok_c = ft_getexpan(shell, tok_h, &tok_c, tok_c->next->content.token);
				if (tok_c)
					tok_n = tok_c->next;
			}
			if (head && tok_c)
			{
				// ft_printerr("TRUE\n");
				shell->token = tok_c;
				head = false;
			}
		}
		else if (tok->type == TOKEN_DQUOTE)
		{
			// ft_printerr("Ping ft_expansion 3: %s, %d\n", tok->value, tok->type);
			ft_expan_dquote(shell, tok_c, status);
		}
		if (!tok_c)
		{
			// ft_printerr("Ping ft_expansion 5\n");
			return (-1);
		}
		// ft_printerr("Ping ft_expansion 6\n");
		tok_c = tok_n;
	}
	// ft_printerr("Ping ft_expansion 7: %s\n", shell->token->content.token->value);
	return (0);
}
