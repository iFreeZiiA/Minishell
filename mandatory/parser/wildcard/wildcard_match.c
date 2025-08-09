/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:55:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Vérifie si le fichier doit être affiché (gestion fichiers cachés)
 * @param filename Nom du fichier
 * @param pattern Pattern de recherche
 * @return int 1 si doit être affiché, 0 sinon
 */
static int	ft_should_show_hidden(const char *filename, const char *pattern)
{
	if (!filename || !pattern)
		return (0);
	if (filename[0] != '.')
		return (1);
	if (pattern[0] == '.')
		return (1);
	return (0);
}

/**
 * @brief Matching récursif pattern/string avec support *
 * @param pattern Pattern avec wildcards
 * @param string String à matcher
 * @return int 1 si match, 0 sinon
 */
static int	ft_handle_star(const char **pattern, const char **string,
			const char **star_pattern, const char **star_string)
{
	if (**pattern == '*')
	{
		*star_pattern = ++(*pattern);
		*star_string = *string;
		return (1);
	}
	else if (*star_pattern)
	{
		*pattern = *star_pattern;
		*string = ++(*star_string);
		return (1);
	}
	return (0);
}

static int	ft_match_recursive(const char *pattern, const char *string)
{
	const char	*star_pattern;
	const char	*star_string;

	star_pattern = NULL;
	star_string = NULL;
	while (*string)
	{
		if (*pattern == *string)
		{
			pattern++;
			string++;
		}
		else if (!ft_handle_star(&pattern, &string, &star_pattern,
				&star_string))
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0');
}

/**
 * @brief Match un pattern contre un nom de fichier
 * @param pattern Pattern avec wildcards potentiels
 * @param filename Nom de fichier à tester
 * @return int 1 si match, 0 sinon
 */
int	ft_match_pattern(const char *pattern, const char *filename)
{
	if (!pattern || !filename)
		return (0);
	if (!ft_should_show_hidden(filename, pattern))
		return (0);
	return (ft_match_recursive(pattern, filename));
}
