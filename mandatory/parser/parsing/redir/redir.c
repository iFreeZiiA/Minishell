/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:49:09 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Parse toutes les redirections d'une commande
 * @param shell Structure shell
 * @param tokens Liste de tokens
 * @param cmd Commande à enrichir
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_redirections(t_shell *shell, t_list *tokens, t_command *cmd)
{
	t_list	*current;
	int		result;

	if (!shell || !tokens || !cmd)
		return (-1);
	if (ft_validate_redirection_syntax(tokens) != 0)
		return (-1);
	current = tokens;
	while (current)
	{
		if (ft_is_redirection_token(current->content.token))
		{
			result = ft_process_single_redirection(current, cmd);
			if (result != 0)
				return (-1);
		}
		current = current->next;
	}
	if (ft_check_redirection_conflicts(cmd->redirs) != 0)
		return (-1);
	return (0);
}

/**
 * @brief Identifie le type de redirection
 * @param token Token de redirection
 * @return redir_type Type identifié
 */
redir_type	ft_get_redirection_type(t_token *token)
{
	if (!token)
		return (-1);
	if (token->type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (token->type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (token->type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	if (token->type == TOKEN_APPEND)
		return (REDIR_APPEND);
	return (-1);
}

/**
 * @brief Trouve le prochain token de redirection
 * @param tokens Liste de tokens
 * @return t_list* Premier token de redirection trouvé
 */
t_list	*ft_find_next_redirection(t_list *tokens)
{
	t_list	*current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (ft_is_redirection_token(current->content.token))
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Crée une structure redirection
 * @param type Type de redirection
 * @param filename Nom du fichier
 * @return t_redir* Nouvelle redirection
 */
t_redir	*ft_create_redirection(redir_type type, char *filename)
{
	t_redir	*redir;

	if (!filename)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(filename);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->fd = -1;
	return (redir);
}
