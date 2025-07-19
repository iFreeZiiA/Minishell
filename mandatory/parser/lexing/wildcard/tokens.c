/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/19 14:30:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Builds token list from matches array
 * 
 * @param matches Array of filenames
 * @param count Number of matches
 * @param token_list Pointer to token list
 * @return bool true on success
 */
bool	ft_build_token_list(char **matches, int count, t_list **token_list)
{
	int			i;
	t_token		*token;
	t_content	content;

	i = 0;
	while (i < count)
	{
		token = ft_create_filename_token(matches[i]);
		if (!token)
		{
			ft_cleanup_failed_token(NULL, matches);
			return (false);
		}
		if (!ft_validate_token(token))
		{
			ft_cleanup_failed_token(token, matches);
			return (false);
		}
		content.token = token;
		if (!ft_lstadd_back(token_list, content, TYPE_TOKEN))
		{
			ft_cleanup_failed_token(token, matches);
			return (false);
		}
		i++;
	}
	return (true);
}

/**
 * @brief Creates token for filename
 * 
 * @param filename Name of file
 * @return t_token* New token
 */
t_token	*ft_create_filename_token(const char *filename)
{
	t_token	*token;

	if (!filename)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup((char *) filename);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = TOKEN_WORD;
	return (token);
}

/**
 * @brief Validates token structure
 * 
 * @param token Token to validate
 * @return bool true if valid
 */
bool	ft_validate_token(t_token *token)
{
	if (!token)
		return (false);
	if (!token->value)
		return (false);
	if (token->type != TOKEN_WORD)
		return (false);
	if (!ft_is_valid_filename(token->value))
		return (false);
	return (true);
}

/**
 * @brief Checks if filename is valid
 * 
 * @param filename Filename to check
 * @return bool true if valid
 */
bool	ft_is_valid_filename(const char *filename)
{
	if (!filename)
		return (false);
	if (ft_strlen(filename) == 0)
		return (false);
	if (ft_strcmp(filename, ".") == 0)
		return (false);
	if (ft_strcmp(filename, "..") == 0)
		return (false);
	return (true);
}

/**
 * @brief Cleans up failed token creation
 * 
 * @param token Token to clean
 * @param matches Matches array to free
 */
void	ft_cleanup_failed_token(t_token *token, char **matches)
{
	int	i;

	if (token)
	{
		if (token->value)
			free(token->value);
		free(token);
	}
	if (matches)
	{
		i = 0;
		while (matches[i])
		{
			free(matches[i]);
			i++;
		}
		free(matches);
	}
}
