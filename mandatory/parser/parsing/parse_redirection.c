/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:37:51 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/11 11:33:00 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Extracts the filename from a token
 * 
 * @param tok Token containing the filename
 * @return char* Duplicated filename string, NULL on error
 */
static char	*ft_get_filename(t_token *tok)
{
	if (!tok)
		return (NULL);
	if (tok->type == TOKEN_VAR || tok->type == TOKEN_WORD)
		return (ft_strdup(tok->value));
	return (NULL);
}

/**
 * @brief Creates a redirection structure with the specified type and file
 * 
 * @param type Redirection type
 * @param file File or delimiter name
 * @return t_redir* New redirection structure, NULL on error
 */
static t_redir	*ft_create_redir(redir_type type, char *file)
{
	t_redir	*redir;
	
	if (!file)
		return (NULL);
	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->fd = -1;
	return (redir);
}

/**
 * @brief Parses a redirection token and creates a redirection structure
 * 
 * @param shell The shell structure
 * @param tok_curr Current token (redirection operator)
 * @return t_redir* New redirection structure, NULL on error
 */
t_redir	*parse_redirection(t_shell *shell, t_list *tok_curr)
{
	t_token		*tok;
	t_token		*file_tok;
	char		*filename;
	redir_type	type;
	
	if (!shell || !tok_curr)
		return (NULL);
	tok = tok_curr->content.token;
	if (!tok_curr->next || !tok_curr->next->content.token)
		return (NULL);
	file_tok = tok_curr->next->content.token;
	filename = ft_get_filename(file_tok);
	if (!filename)
		return (NULL);
	if (tok->type == TOKEN_REDIR_IN)
		type = REDIR_IN;
	else if (tok->type == TOKEN_REDIR_OUT)
		type = REDIR_OUT;
	else if (tok->type == TOKEN_HEREDOC)
		type = REDIR_HEREDOC;
	else if (tok->type == TOKEN_APPEND)
		type = REDIR_APPEND;
	else
	{
		free(filename);
		return (NULL);
	}
	return (ft_create_redir(type, filename));
}

/**
 * @brief Processes heredoc redirections
 * 
 * @param shell The shell structure
 * @param redir Redirection structure
 * @return int File descriptor for the heredoc, -1 on error
 */
int	process_heredoc(t_shell *shell, t_redir *redir)
{
	char	*line;
	int		pipe_fd[2];
	
	if (!shell || !redir || redir->type != REDIR_HEREDOC)
		return (-1);
	if (pipe(pipe_fd) < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

/**
 * @brief Adds a redirection to a command
 * 
 * @param cmd Command to add redirection to
 * @param redir Redirection to add
 * @return int 0 on success, -1 on error
 */
int	add_redirection(t_command *cmd, t_redir *redir)
{
	if (!cmd || !redir)
		return (-1);
	if (!ft_lstadd_back(&cmd->redirs, (t_content){.redir = redir}, TYPE_REDIR))
		return (-1);
	return (0);
}

/**
 * @brief Collects all redirections for a command
 * 
 * @param shell The shell structure
 * @param start First token of the command
 * @param end Last token of the command
 * @param cmd Command to add redirections to
 * @return int Number of redirections added, -1 on error
 */
int	collect_redirections(t_shell *shell, t_list *start, t_list *end, t_command *cmd)
{
	t_list	*curr;
	t_redir	*redir;
	int		count;
	
	if (!shell || !start || !cmd)
		return (-1);
	curr = start;
	count = 0;
	while (curr && curr != end)
	{
		if (curr->content.token->type == TOKEN_REDIR_IN || 
			curr->content.token->type == TOKEN_REDIR_OUT ||
			curr->content.token->type == TOKEN_HEREDOC ||
			curr->content.token->type == TOKEN_APPEND)
		{
			redir = parse_redirection(shell, curr);
			if (!redir)
				return (-1);
			if (redir->type == REDIR_HEREDOC)
				redir->fd = process_heredoc(shell, redir);
			if (add_redirection(cmd, redir) < 0)
			{
				free(redir->file);
				free(redir);
				return (-1);
			}
			count++;
			curr = curr->next;
		}
		curr = curr->next;
	}
	return (count);
}