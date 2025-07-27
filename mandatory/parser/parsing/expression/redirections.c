/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/06/17 18:59:49 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Converts a redirection token type to its corresponding redirection type
 * 
 * This function maps token types for redirection operators to their
 * equivalent redirection types used in the command structure.
 * 
 * @param token_type Token type of the redirection operator
 * @return redir_type Corresponding redirection type
 */
static redir_type	ft_token_to_redir_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (token_type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	if (token_type == TOKEN_APPEND)
		return (REDIR_APPEND);
	return (REDIR_IN);
}

/**
 * @brief Creates a new redirection structure
 * 
 * This function allocates and initializes a redirection structure
 * with the specified type and filename. The file descriptor is
 * initialized to -1 (unopened).
 * 
 * @param type Type of redirection (input, output, heredoc, append)
 * @param filename Name of the file for redirection
 * @return t_redir* Pointer to created redirection, or NULL on failure
 */
static t_redir	*ft_create_redirection(redir_type type, char *filename)
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

/**
 * @brief Adds a redirection to a command's redirection list
 * 
 * This function adds a redirection structure to the command's
 * list of redirections using the linked list management functions.
 * 
 * @param cmd Command to add the redirection to
 * @param redir Redirection structure to add
 * @return int 0 on success, -1 on failure
 */
static int	ft_add_redirection_to_cmd(t_command *cmd, t_redir *redir)
{
	t_content	content;

	if (!cmd || !redir)
		return (-1);
	content.redir = redir;
	if (!ft_lstadd_back(&cmd->redirs, content, TYPE_REDIR))
		return (-1);
	return (0);
}

/**
 * @brief Processes a single redirection token pair
 * 
 * This function handles a redirection operator and its associated filename:
 * 1. Validates that a filename follows the operator
 * 2. Creates a redirection structure
 * 3. Adds it to the command's redirection list
 * 
 * @param curr Current token (redirection operator)
 * @param cmd Command to add the redirection to
 * @return int 0 on success, -1 on failure
 */
static int	ft_process_single_redirection(t_list *curr, t_command *cmd)
{
	t_redir		*redir;
	redir_type	type;

	if (!curr->next)
		return (-1);
	type = ft_token_to_redir_type(curr->content.token->type);
	redir = ft_create_redirection(type, curr->next->content.token->value);
	if (!redir)
		return (-1);
	if (ft_add_redirection_to_cmd(cmd, redir) < 0)
	{
		free(redir->file);
		free(redir);
		return (-1);
	}
	return (0);
}

/**
 * @brief Parses all redirections in a token list and adds them to a command
 * 
 * This function traverses the entire token list, identifies redirection
 * operators, and processes each redirection pair (operator + filename).
 * All found redirections are added to the command's redirection list.
 * 
 * @param shell Shell structure for context
 * @param token_h Head of the token list to process
 * @param cmd Command to add redirections to
 * @return int 0 on success, -1 on failure
 */
int	ft_parse_redirections(t_shell *shell, t_list *token_h, t_command *cmd)
{
	t_list	*curr;

	if (!shell || !token_h || !cmd)
		return (-1);
	curr = token_h;
	while (curr)
	{
		if (ft_is_redirect_token(curr->content.token->type))
		{
			if (ft_process_single_redirection(curr, cmd) < 0)
				return (-1);
			curr = curr->next;
		}
		curr = curr->next;
	}
	return (0);
}
