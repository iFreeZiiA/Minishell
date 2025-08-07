/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/07 23:33:39 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "../minishell.h"
# include <dirent.h>
# include <sys/stat.h>

/* ----------------------------- WILDCARD MAIN ----------------------------- */

void	ft_expand_wildcards(t_shell *shell, t_list **token_list);
int		ft_needs_wildcard_expansion(t_token *token);
int		ft_replace_wildcard_token(t_list **token_list, t_list *current);

/* ---------------------------- WILDCARD MATCH ----------------------------- */

int		ft_match_pattern(const char *pattern, const char *filename);

/* ---------------------------- WILDCARD FILES ----------------------------- */

char	**ft_get_matching_files(const char *pattern);

/* --------------------------- WILDCARD UTILS ----------------------------- */

void	ft_free_string_array(char **array);
void	ft_free_token(t_token *token);
t_token	*ft_create_filename_token(char *filename);
t_list	*ft_create_match_tokens(char **matches);

#endif
