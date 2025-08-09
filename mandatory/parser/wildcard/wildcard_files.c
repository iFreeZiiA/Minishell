/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:56:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"
#include <dirent.h>

/**
 * @brief Ajoute un match au tableau de résultats
 * @param matches Pointeur vers le tableau de matches
 * @param count Pointeur vers le compteur
 * @param capacity Pointeur vers la capacité
 * @param filename Nom de fichier à ajouter
 * @return int 0 succès, -1 erreur
 */
static int	ft_add_match(char ***matches, int *count, int *capacity,
		char *filename)
{
	char	**new_matches;
	int		new_capacity;
	int		i;

	if (*count >= *capacity - 1)
	{
		new_capacity = (*capacity) * 2;
		new_matches = malloc(sizeof(char *) * new_capacity);
		if (!new_matches)
			return (-1);
		i = 0;
		while (i < *count)
		{
			new_matches[i] = (*matches)[i];
			i++;
		}
		free(*matches);
		*matches = new_matches;
		*capacity = new_capacity;
	}
	(*matches)[*count] = ft_strdup(filename);
	if (!(*matches)[*count])
		return (-1);
	(*count)++;
	return (0);
}

/**
 * @brief Trie les matches par ordre alphabétique
 * @param matches Tableau de strings
 * @param count Nombre d'éléments
 */
static void	ft_sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				temp = matches[i];
				matches[i] = matches[j];
				matches[j] = temp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Collecte les fichiers correspondant au pattern
 * @param pattern Pattern de recherche
 * @return char** Tableau de noms de fichiers (NULL-terminated)
 */
char	**ft_get_matching_files(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;
	int				capacity;

	capacity = 64;
	count = 0;
	matches = malloc(sizeof(char *) * capacity);
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (matches);
	entry = readdir(dir);
	while (entry)
	{
		if (ft_match_pattern(pattern, entry->d_name))
			if (ft_add_match(&matches, &count, &capacity, entry->d_name) == -1)
				break ;
		entry = readdir(dir);
	}
	closedir(dir);
	matches[count] = NULL;
	if (count > 0)
		ft_sort_matches(matches, count);
	return (matches);
}
