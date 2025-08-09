/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 11:13:52 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_add_redir_to_cmd(t_redir *redirection,
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

int	ft_proc_single_redir(t_list *redir_token, t_command *cmd)
{
	t_token		*token;
	char		*filename;
	t_redir		*redirection;

	if (!redir_token || !redir_token->content.token || !cmd)
		return (-1);
	token = redir_token->content.token;
	filename = ft_ext_redir_file(redir_token);
	if (!filename)
		return (-1);
	redirection = ft_new_redir(ft_get_t_redir_type(token),
			filename);
	if (!redirection)
	{
		free(filename);
		return (-1);
	}
	return (ft_add_redir_to_cmd(redirection, cmd, filename));
}

/**
 * @brief Parse toutes les redirections d'une commande
 * @param shell Structure shell
 * @param tokens Liste de tokens
 * @param cmd Commande à enrichir
 * @return int 0 succès, -1 erreur
 */
int	ft_parse_redir(t_shell *shell, t_list *tokens, t_command *cmd)
{
	t_list	*current;
	int		result;

	if (!shell || !tokens || !cmd)
		return (-1);
	if (ft_val_redir_syntax(tokens) != 0)
		return (-1);
	current = tokens;
	while (current)
	{
		if (ft_is_redir_tok(current->content.token))
		{
			result = ft_proc_single_redir(current, cmd);
			if (result != 0)
				return (-1);
		}
		current = current->next;
	}
	if (ft_check_redir_conf(cmd->redirs) != 0)
		return (-1);
	return (0);
}

/**
 * @brief Identifie le type de redirection
 * @param token Token de redirection
 * @return t_redir_type Type identifié
 */
t_redir_type	ft_get_t_redir_type(t_token *token)
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
t_list	*ft_find_next_redir(t_list *tokens)
{
	t_list	*current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (ft_is_redir_tok(current->content.token))
			return (current);
		current = current->next;
	}
	return (NULL);
}
