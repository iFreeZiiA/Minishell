/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:57:37 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/11 13:28:13 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	*ft_get_str(t_list *tok_start, t_token *tok_next)
{
	t_list	*current;
	t_token	*tok;
	char	*str;
	
	current = tok_start;
	str = NULL;
	while (current->content.token->value != tok_next->value)
	{
		tok = (t_token *) current->content.token;
		str = ft_strjoin_f(str, tok->value);
		tok_start = current->next;
		ft_clean_node_tok(current);
		current = tok_start;
	}
	if (!current)
		return (NULL);
	return (str);
}

bool	ft_new_list(t_list *tok_start, t_list *tok_next)
{
	t_list	*first;
	t_token	*new;
	char	*str;

	if (!tok_start || !tok_next)
		return (false);
	first = tok_start->prev;
	str = ft_get_str(tok_start, (t_token *) tok_next->content.token);
	if (!str)
	{
		ft_lstleak(&first, ft_clean_node_tok);
		return (false);
	}
	new = (t_token *) malloc(sizeof(t_token));
	new->type = TOKEN_DQUOTE;
	new->value = str;
	first->next = ft_lstnew((void *) new);
	first->next->next = tok_next;
	return (true);
}

// t_list	*ft_get_end(t_shell *shell, t_list *current, t_token_type type, int i)
// {
// 	t_list	*start;
// 	t_list	*var;
// 	t_token	*tok;
// 	char	*value;
// 	bool	d_var;

// 	d_var = false;
// 	value = NULL;
// 	start = current->next->next;
// 	tok = start->content.token;
// 	while (tok->type != type)
// 	{
// 		if (type == TOKEN_DQUOTE)
// 		{
// 			if (tok->type == TOKEN_STATUS)
// 			{
// 				tok->type = TOKEN_WORD;
// 				tok->value = ft_itoa(shell->env->last_exit_code);
// 			}
// 			else if (tok->type == TOKEN_VAR)
// 			{
// 				var = start;
// 				d_var = true;
// 			}
// 		}
// 		if (d_var)
// 		{
// 			if (tok->type == TOKEN_WORD)
// 			{
// 				value = ft_isenv(shell->env->env_vars, tok->value);
// 				if (!value)
// 				{
// 					value = ft_isenv(shell->env->local_env, tok->value);
// 					if (!value)
// 					{
// 						var->prev->next = start->next;
// 						start->next->prev = var->prev;
// 						ft_clean_node_tok(start);
// 						start = var->prev;
// 						ft_clean_node_tok(var);
// 						tok = (t_token *) malloc(sizeof(t_token));
// 						tok->type = TOKEN_WORD;
// 						tok->value = "";
// 						var = ft_lstnew_tok(tok);
// 						if (!var)
// 							return (NULL);
// 						ft_lstinsert(&shell->token, var, i - 1);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				tok->type = TOKEN_WORD;
// 				d_var = false;
// 			}
// 		}
// 		start = start->next;
// 		tok = start->content.token;
// 	}
// 	return (start);
// }

t_token	*ft_new_token(char *content, t_token_type type)
{
	t_token	*tok;

	tok = (t_token *) malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = content;
	return (tok);
}

int	ft_getindex_quote(t_shell *shell, t_list *curr, t_token_type type, int index)
{
	t_list			*temp;
	t_token			*new;
	t_token_type	*prev;
	char			*var_value;

	temp = curr->next;
	prev = curr->content.token->type;
	while (temp)
	{
		index++;
		if (type == temp->content.token->type && prev != TOKEN_BSLASH)
			return (index);
		if (type == TOKEN_DQUOTE)
		{
			if (temp->content.token->type == TOKEN_VAR)
			{
				var_value = ft_getenv_value(shell, temp->content.token->value);
				if (!var_value)
					
			}
		}
		prev = temp->content.token->type;
		temp = temp->next;
	}
}

t_list	*ft_get_end(t_shell *shell, t_list *curr, t_token_type type, int index)
{
	t_list	*new;
	t_list	*temp;
	int		i;
	char	*str;

	if (!shell || !curr || !type || index < 0)
		return (NULL);
	temp = curr->next;
	while (temp)
	{
		if (temp->content.token->type == )
		temp = temp->next;
	}
}

int	ft_lexer_checker(t_shell *shell)
{
	t_list	*tok_current;
	t_token	*tok;
	int		i;

	tok_current = shell->token;
	i = 0;
	while (tok_current)
	{
		tok = (t_token *) tok_current->next->content.token;
		if (!tok)
			return (-1);
		if (tok->type == TOKEN_QUOTE || tok->type == TOKEN_DQUOTE)
			ft_get_end(tok_current, ft_get_end(shell, tok_current, tok->type, i));
		tok_current = tok_current->next;
		i++;
	}
	return (0);
}