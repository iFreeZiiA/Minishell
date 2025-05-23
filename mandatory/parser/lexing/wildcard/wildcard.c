/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:18:03 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/23 22:53:46 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"
#include <dirent.h>
#include <sys/stat.h>

/**
 * @brief Checks if a character is a wildcard character
 * 
 * @param c Character to check
 * @return bool true if wildcard, false otherwise
 */
static bool	ft_is_wildcard_char(char c)
{
	return (c == '*');
}

/**
 * @brief Matches a string against a pattern with wildcards
 * 
 * @param pattern Pattern with potential wildcards
 * @param string String to match
 * @return bool true if matches, false otherwise
 */
static bool	ft_match_pattern(const char *pattern, const char *string)
{
	const char	*p;
	const char	*s;
	const char	*star_p;
	const char	*star_s;

	p = pattern;
	s = string;
	star_p = NULL;
	star_s = NULL;
	
	while (*s)
	{
		if (*p == '*')
		{
			star_p = p++;
			star_s = s;
		}
		else if (*p == *s)
		{
			p++;
			s++;
		}
		else if (star_p)
		{
			p = star_p + 1;
			s = ++star_s;
		}
		else
			return (false);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}

/**
 * @brief Checks if filename should be hidden (starts with .)
 * 
 * @param filename The filename to check
 * @param pattern The pattern being matched
 * @return bool true if should show hidden files, false otherwise
 */
static bool	ft_should_show_hidden(const char *filename, const char *pattern)
{
	if (filename[0] != '.')
		return (true);
	if (pattern[0] == '.')
		return (true);
	return (false);
}

/**
 * @brief Gets all files matching a pattern in current directory
 * 
 * @param pattern Pattern to match
 * @return char** Array of matching filenames, NULL on error
 */
static char	**ft_get_matching_files(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;
	int				capacity;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	
	capacity = 16;
	matches = (char **)malloc(sizeof(char *) * capacity);
	if (!matches)
	{
		closedir(dir);
		return (NULL);
	}
	
	count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (!ft_should_show_hidden(entry->d_name, pattern))
			continue;
		
		if (ft_match_pattern(pattern, entry->d_name))
		{
			if (count >= capacity - 1)
			{
				capacity *= 2;
				char **new_matches = ft_realloc(matches, 
					sizeof(char *) * capacity, sizeof(char *) * (capacity / 2));
				if (!new_matches)
				{
					ft_free_arr(matches);
					closedir(dir);
					return (NULL);
				}
				matches = new_matches;
			}
			
			matches[count] = ft_strdup(entry->d_name);
			if (!matches[count])
			{
				ft_free_arr(matches);
				closedir(dir);
				return (NULL);
			}
			count++;
		}
	}
	
	matches[count] = NULL;
	closedir(dir);
	
	if (count == 0)
	{
		free(matches);
		return (NULL);
	}
	
	return (matches);
}

/**
 * @brief Sorts an array of strings alphabetically
 * 
 * @param arr Array to sort
 * @param size Size of the array
 */
static void	ft_sort_matches(char **arr, int size)
{
	int		i;
	int		j;
	char	*temp;

	if (!arr || size <= 1)
		return;
	
	for (i = 0; i < size - 1; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

/**
 * @brief Creates new tokens from matched filenames
 * 
 * @param matches Array of matched filenames
 * @return t_list* List of new tokens, NULL on error
 */
static t_list	*ft_create_match_tokens(char **matches)
{
	t_list	*token_list;
	t_token	*token;
	int		i;
	int		count;

	if (!matches)
		return (NULL);
	
	count = ft_count_arr(matches);
	if (count == 0)
		return (NULL);
	
	ft_sort_matches(matches, count);
	
	token_list = NULL;
	for (i = 0; i < count; i++)
	{
		token = (t_token *)malloc(sizeof(t_token));
		if (!token)
		{
			ft_lstfree_t(token_list);
			return (NULL);
		}
		
		token->type = TOKEN_WORD;
		token->value = ft_strdup(matches[i]);
		if (!token->value)
		{
			free(token);
			ft_lstfree_t(token_list);
			return (NULL);
		}
		
		if (!ft_lstadd_back(&token_list, (t_content){.token = token}, TYPE_TOKEN))
		{
			free(token->value);
			free(token);
			ft_lstfree_t(token_list);
			return (NULL);
		}
	}
	
	return (token_list);
}

/**
 * @brief Replaces a wildcard token with matched filenames
 * 
 * @param tok_h Pointer to token list head
 * @param tok_curr Current wildcard token
 * @return bool true on success, false on error
 */
static bool	ft_replace_wildcard_token(t_list **tok_h, t_list *tok_curr)
{
	char	**matches;
	t_list	*new_tokens;
	t_list	*last_new;
	t_token	*token;

	if (!tok_h || !tok_curr || !tok_curr->content.token)
		return (false);
	
	token = tok_curr->content.token;
	if (token->type != TOKEN_WILDCARD)
		return (true);
	
	// Si le wildcard est juste "*", chercher tous les fichiers
	// Sinon, utiliser la valeur du token comme pattern
	if (ft_strcmp(token->value, "*") == 0)
		matches = ft_get_matching_files("*");
	else
		matches = ft_get_matching_files(token->value);
	
	// Si aucun match, garder le token original
	if (!matches)
		return (true);
	
	new_tokens = ft_create_match_tokens(matches);
	ft_free_arr(matches);
	
	if (!new_tokens)
		return (false);
	
	// Remplacer le token wildcard par les nouveaux tokens
	last_new = ft_lstlast(new_tokens);
	
	// Connecter le début de la nouvelle liste
	new_tokens->prev = tok_curr->prev;
	if (tok_curr->prev)
		tok_curr->prev->next = new_tokens;
	else
		*tok_h = new_tokens;
	
	// Connecter la fin de la nouvelle liste
	last_new->next = tok_curr->next;
	if (tok_curr->next)
		tok_curr->next->prev = last_new;
	
	// Libérer l'ancien token
	ft_clean_node_tok(tok_curr);
	
	return (true);
}

/**
 * @brief Checks if a token contains wildcards that need expansion
 * 
 * @param token Token to check
 * @return bool true if needs expansion, false otherwise
 */
static bool	ft_needs_wildcard_expansion(t_token *token)
{
	int	i;

	if (!token || !token->value)
		return (false);
	
	if (token->type == TOKEN_WILDCARD)
		return (true);
	
	// Check if TOKEN_WORD contains wildcards
	if (token->type == TOKEN_WORD)
	{
		i = 0;
		while (token->value[i])
		{
			if (ft_is_wildcard_char(token->value[i]))
				return (true);
			i++;
		}
	}
	
	return (false);
}

/**
 * @brief Main wildcard expansion function
 * 
 * @param shell The shell structure
 * @param tok_h Pointer to token list head
 * @return int 0 on success, -1 on error
 */
int	ft_expand_wildcard(t_shell *shell, t_list **tok_h)
{
	t_list	*tok_curr;
	t_list	*tok_next;
	t_token	*token;

	if (!shell || !tok_h || !*tok_h)
		return (0);
	
	tok_curr = *tok_h;
	while (tok_curr)
	{
		tok_next = tok_curr->next;
		token = tok_curr->content.token;
		
		if (ft_needs_wildcard_expansion(token))
		{
			// Si c'est un TOKEN_WORD avec des wildcards, le convertir
			if (token->type == TOKEN_WORD)
				token->type = TOKEN_WILDCARD;
			
			if (!ft_replace_wildcard_token(tok_h, tok_curr))
				return (-1);
		}
		
		tok_curr = tok_next;
	}
	
	return (0);
}

/**
 * @brief Expands wildcards in the token list (public interface)
 * 
 * @param shell The shell structure
 * @param tok_h Pointer to token list head
 */
void	ft_exp_wildcard(t_shell *shell, t_list **tok_h)
{
	if (ft_expand_wildcard(shell, tok_h) < 0)
	{
		ft_printerr("minishell: wildcard expansion failed\n");
		shell->env->last_exit_code = 1;
	}
}