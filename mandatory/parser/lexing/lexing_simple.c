/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:20:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/02 12:33:52 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

static int	ft_is_operator(char c)
{
	if (c == '|' || c == '&' || c == '>' || c == '<')
		return (1);
	return (0);
}

static int	ft_is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

static void	ft_skip_spaces(char *line, int *i)
{
	while (line[*i] && ft_is_space(line[*i]))
		(*i)++;
}

static char	*ft_extract_word(char *line, int *i)
{
	int		start;
	int		len;
	char	*word;
	
	start = *i;
	while (line[*i] && !ft_is_space(line[*i]) && !ft_is_operator(line[*i]))
	{
		if (line[*i] == '"')
		{
			(*i)++;
			while (line[*i] && line[*i] != '"')
				(*i)++;
			if (line[*i] == '"')
				(*i)++;
		}
		else if (line[*i] == '\'')
		{
			(*i)++;
			while (line[*i] && line[*i] != '\'')
				(*i)++;
			if (line[*i] == '\'')
				(*i)++;
		}
		else
			(*i)++;
	}
	len = *i - start;
	word = ft_substr(line, start, len);
	return (word);
}

static t_token	*ft_create_token(char *value, t_token_type type)
{
	t_token	*token;
	
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	return (token);
}

static char	*ft_extract_operator(char *line, int *i)
{
	char	*op;
	int		start;
	
	start = *i;
	if (line[*i] == '|' && line[*i + 1] == '|')
		*i += 2;
	else if (line[*i] == '&' && line[*i + 1] == '&')
		*i += 2;
	else if (line[*i] == '>' && line[*i + 1] == '>')
		*i += 2;
	else if (line[*i] == '<' && line[*i + 1] == '<')
		*i += 2;
	else
		(*i)++;
	op = ft_substr(line, start, *i - start);
	return (op);
}

static t_token_type	ft_get_operator_type(char *op)
{
	if (!ft_strcmp(op, "|"))
		return (TOKEN_PIPE);
	if (!ft_strcmp(op, "||"))
		return (TOKEN_OR);
	if (!ft_strcmp(op, "&&"))
		return (TOKEN_AND);
	if (!ft_strcmp(op, ">"))
		return (TOKEN_REDIR_OUT);
	if (!ft_strcmp(op, "<"))
		return (TOKEN_REDIR_IN);
	if (!ft_strcmp(op, ">>"))
		return (TOKEN_APPEND);
	if (!ft_strcmp(op, "<<"))
		return (TOKEN_HEREDOC);
	return (TOKEN_WORD);
}

static int	ft_add_token(t_list **tokens, char *value, t_token_type type)
{
	t_content	content;
	t_token		*token;
	
	token = ft_create_token(value, type);
	if (!token)
		return (-1);
	content.token = token;
	ft_lstadd_back(tokens, content, TYPE_TOKEN);
	return (0);
}

t_list	*ft_lexing_simple(t_shell *shell)
{
	t_list	*tokens;
	char	*line;
	char	*word;
	char	*op;
	int		i;
	
	if (!shell || !shell->current_line)
		return (NULL);
	tokens = NULL;
	line = shell->current_line;
	i = 0;
	while (line[i])
	{
		ft_skip_spaces(line, &i);
		if (!line[i])
			break;
		if (ft_is_operator(line[i]))
		{
			op = ft_extract_operator(line, &i);
			if (ft_add_token(&tokens, op, ft_get_operator_type(op)) == -1)
				return (ft_lstfree_t(tokens));
		}
		else
		{
			word = ft_extract_word(line, &i);
			if (ft_add_token(&tokens, word, TOKEN_WORD) == -1)
				return (ft_lstfree_t(tokens));
		}
	}
	return (tokens);
}

static int	ft_env_var_match(char *env_line, char *var_name)
{
	int	i;
	
	i = 0;
	while (var_name[i] && env_line[i])
	{
		if (var_name[i] != env_line[i])
			return (0);
		i++;
	}
	// Vérifier que var_name est entièrement matché et suivi par =
	if (var_name[i] == '\0' && env_line[i] == '=')
		return (1);
	return (0);
}

static char	*ft_expand_variable_with_shell(char *str, char **envp, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	int		i;
	
	if (!str || str[0] != '$' || !envp)
		return (ft_strdup(str));
	var_name = str + 1;  // Skip $
	
	// Gestion spéciale pour $?
	if (ft_strcmp(var_name, "?") == 0)
	{
		if (shell && shell->env)
			result = ft_itoa(shell->env->last_exit_code);
		else
			result = ft_strdup("0");
		return (result);
	}
	
	i = 0;
	while (envp[i])
	{
		if (ft_env_var_match(envp[i], var_name))
		{
			var_value = envp[i] + ft_strlen(var_name) + 1;
			result = ft_strdup(var_value);
			return (result);
		}
		i++;
	}
	return (ft_strdup(""));  // Variable not found
}



static char	*ft_remove_quotes(char *str)
{
	int		len;
	char	*result;
	
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len-1] == '"')
		|| (str[0] == '\'' && str[len-1] == '\'')))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

static void	ft_expand_tokens(t_list *tokens, char **envp, t_shell *shell)
{
	t_list	*current;
	t_token	*token;
	char	*expanded;
	char	*cleaned;
	
	if (!envp)
		return;
	current = tokens;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (token->type == TOKEN_WORD)
			{
				expanded = ft_expand_variable_with_shell(token->value, envp, shell);
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
