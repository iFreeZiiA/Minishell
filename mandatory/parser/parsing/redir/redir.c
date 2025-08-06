/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 21:47:42 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_add_redirection_to_command(t_redir *redirection,
		t_command *cmd, char *filename)
{
	t_content	content;

	content.redir = redirection;
	if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
	{
		free(filename);
		return (-1);
	}
	free(filename);
	return (0);
}

int	ft_process_single_redirection(t_list *redir_token, t_command *cmd)
{
	t_token		*token;
	char		*filename;
	t_redir		*redirection;

	if (!redir_token || !redir_token->content.token || !cmd)
		return (-1);
	token = redir_token->content.token;
	filename = ft_extract_redirection_file(redir_token);
	if (!filename)
		return (-1);
	redirection = ft_create_redirection(ft_get_redirection_type(token),
			filename);
	if (!redirection)
	{
		free(filename);
		return (-1);
	}
	return (ft_add_redirection_to_command(redirection, cmd, filename));
}

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
