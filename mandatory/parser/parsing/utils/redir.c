/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:23:50 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Compte redirections dans liste tokens
 * @param tokens Liste de tokens
 * @return int Nombre de redirections
 */
int	ft_count_redirections(t_list *tokens)
{
	t_list	*current;
	int		count;

	if (!tokens)
		return (0);
	current = tokens;
	count = 0;
	while (current)
	{
		if (ft_is_redirection_token(current->content.token))
			count++;
		current = current->next;
	}
	return (count);
}

/**
 * @brief Extrait fichier après token redirection
 * @param redir_token Token de redirection
 * @return char* Nom du fichier ou NULL
 */
char	*ft_extract_redirection_file(t_list *redir_token)
{
	t_list	*next_token;

	if (!redir_token)
		return (NULL);
	next_token = redir_token->next;
	if (!next_token || !next_token->content.token)
		return (NULL);
	if (next_token->content.token->type != TOKEN_WORD)
		return (NULL);
	return (ft_strdup(next_token->content.token->value));
}

/**
 * @brief Vérifie si token est une redirection
 * @param token Token à vérifier
 * @return int 1 si redirection, 0 sinon
 */
int	ft_is_redirection_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TOKEN_REDIR_IN)
		return (1);
	if (token->type == TOKEN_REDIR_OUT)
		return (1);
	if (token->type == TOKEN_HEREDOC)
		return (1);
	if (token->type == TOKEN_APPEND)
		return (1);
	return (0);
}

/**
 * @brief Traite une redirection unique
 * @param redir_token Token de redirection
 * @param cmd Commande cible
 * @return int 0 succès, -1 erreur
 */
int	ft_process_single_redirection(t_list *redir_token, t_command *cmd)
{
	t_token_type	type;

	if (!redir_token || !cmd)
		return (-1);
	type = redir_token->content.token->type;
	if (type == TOKEN_REDIR_IN)
		return (ft_parse_input_redirection(redir_token, cmd));
	if (type == TOKEN_REDIR_OUT)
		return (ft_parse_output_redirection(redir_token, cmd));
	if (type == TOKEN_HEREDOC)
		return (ft_parse_heredoc_redirection(redir_token, cmd));
	if (type == TOKEN_APPEND)
		return (ft_parse_append_redirection(redir_token, cmd));
	return (-1);
}

/**
 * @brief Extrait répertoire d'un chemin de fichier
 * @param filepath Chemin complet
 * @return char* Répertoire ou NULL
 */
char	*ft_extract_directory(char *filepath)
{
	char	*last_slash;
	int		dir_len;

	if (!filepath)
		return (NULL);
	last_slash = ft_strrchr(filepath, '/');
	if (!last_slash)
		return (ft_strdup("."));
	dir_len = last_slash - filepath;
	if (dir_len == 0)
		return (ft_strdup("/"));
	return (ft_substr(filepath, 0, dir_len));
}

/**
 * @brief Vérifie si redirection est d'entrée
 * @param redir Redirection à vérifier
 * @return int 1 si entrée, 0 sinon
 */
int	ft_is_input_redirection(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_IN)
		return (1);
	if (redir->type == REDIR_HEREDOC)
		return (1);
	return (0);
}

/**
 * @brief Vérifie si redirection est de sortie
 * @param redir Redirection à vérifier
 * @return int 1 si sortie, 0 sinon
 */
int	ft_is_output_redirection(t_redir *redir)
{
	if (!redir)
		return (0);
	if (redir->type == REDIR_OUT)
		return (1);
	if (redir->type == REDIR_APPEND)
		return (1);
	return (0);
}

/**
 * @brief Libère liste redirections
 * @param redirs Liste à libérer
 */
void	ft_free_redirection_list(t_list *redirs)
{
	t_list	*current;
	t_list	*next;

	if (!redirs)
		return ;
	current = redirs;
	while (current)
	{
		next = current->next;
		if (current->type == TYPE_REDIR && current->content.redir)
		{
			free(current->content.redir->file);
			free(current->content.redir);
		}
		free(current);
		current = next;
	}
}
