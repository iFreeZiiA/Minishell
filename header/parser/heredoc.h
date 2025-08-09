/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/26 09:25:44 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "../minishell.h"

/**
 * @brief Traite un heredoc complet
 * @param redir Redirection heredoc
 * @return int 0 succès, -1 erreur
 */
int		ft_handle_heredoc(t_redir *redir);

/**
 * @brief Traite tous les heredocs d'une commande
 * @param cmd Commande contenant les redirections
 * @return int 0 succès, -1 erreur
 */
int		ft_process_heredocs(t_command *cmd);

/**
 * @brief Lit une ligne du heredoc depuis stdin
 * @param delimiter Délimiteur de fin
 * @return char* Ligne lue ou NULL si délimiteur trouvé
 */
char	*ft_read_heredoc_line(char *delimiter);

/**
 * @brief Écrit le contenu dans le pipe
 * @param content Contenu à écrire
 * @param write_fd Descripteur d'écriture
 * @return int 0 succès, -1 erreur
 */
int		ft_write_to_pipe(char *content, int write_fd);

/**
 * @brief Accumule les lignes du heredoc
 * @param content Contenu existant
 * @param line Nouvelle ligne
 * @return char* Nouveau contenu
 */
char	*ft_append_line(char *content, char *line);

#endif
