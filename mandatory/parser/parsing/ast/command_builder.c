/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 20:41:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

static int	ft_process_redirections(t_shell *shell, t_list *clean_tokens,
								t_command *cmd)
{
	if (ft_has_redirections_until_pipe(clean_tokens))
	{
		if (ft_parse_redirections(shell, clean_tokens, cmd) != 0)
			return (-1);
	}
	return (0);
}

static char	**ft_extract_args_from_tokens(t_list *clean_tokens, int word_count)
{
	char	**args;

	args = ft_extract_command_args_until_pipe(clean_tokens, word_count);
	return (args);
}

t_ast_node	*ft_create_command_with_redirections(t_list *tokens, t_shell *shell)
{
	t_ast_node	*node;
	t_command	*cmd;
	char		**args;
	int			word_count;
	t_list		*clean_tokens;

	clean_tokens = ft_create_clean_token_list(tokens);
	if (!clean_tokens)
		return (NULL);
	word_count = ft_count_word_tokens_until_pipe(clean_tokens);
	if (word_count == 0)
	{
		ft_free_token_list(clean_tokens);
		return (NULL);
	}
	args = ft_extract_args_from_tokens(clean_tokens, word_count);
	if (!args)
	{
		ft_free_token_list(clean_tokens);
		return (NULL);
	}
	node = ft_create_simple_command_node(args);
	if (!node)
	{
		ft_free_token_list(clean_tokens);
		return (NULL);
	}
	cmd = (t_command *)node->data;
	if (ft_process_redirections(shell, clean_tokens, cmd) != 0)
	{
		ft_free_ast(node);
		ft_free_token_list(clean_tokens);
		return (NULL);
	}
	ft_free_token_list(clean_tokens);
	return (node);
}
