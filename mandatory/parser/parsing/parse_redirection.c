/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:37:51 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 13:56:37 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

char	*ft_getfile(t_token *tok)
{
	t_token_type	type;

	type = tok->type;
	if (type == TOKEN_VAR || type == )
}

t_redir	*ft_getredir(t_shell *shell, t_list *tok_surr, int red)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	if (red == 1)
	{
		if ()
		redir->file = 
	}
}

t_redir	*parse_redirection(t_shell *shell, t_list *tok_curr)
{
	t_token	*tok;
	
	if (!shell || !tok_curr)
		return (NULL);
	tok = tok_curr->content.token;
	if (tok->type == TOKEN_APPEND)
		ft_getredir(shell, tok_curr, 1);
	if (tok->type == TOKEN_REDIR_OUT)
		ft_getredir(shell, tok_curr, 1);
	if (tok->type == TOKEN_HEREDOC)
		ft_getredir(shell, tok_curr, -1);
	if (tok->type == TOKEN_REDIR_IN)
		ft_getredir(shell, tok_curr, -1);
}
