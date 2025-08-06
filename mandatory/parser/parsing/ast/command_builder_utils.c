/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 21:17:07 by jjorda           ###   ########.fr       */
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

static t_ast_node	*ft_create_node_with_cleanup(char **args, t_list *tokens)
{
	t_ast_node	*node;

	node = ft_create_simple_command_node(args);
	if (!node)
	{
		ft_free_token_list(tokens);
		return (NULL);
	}
	return (node);
}

static int	ft_setup_command_tokens(t_list *tokens, t_list **clean_tokens,
			int *word_count)
{
	*clean_tokens = ft_create_clean_token_list(tokens);
	if (!*clean_tokens)
		return (-1);
	*word_count = ft_count_word_tokens_until_pipe(*clean_tokens);
	if (*word_count == 0)
	{
		ft_free_token_list(*clean_tokens);
		return (-1);
	}
	return (0);
}

int	ft_process_command_redirections(t_shell *shell, t_list *clean_tokens,
		t_ast_node *node)
{
	t_command	*cmd;

	cmd = (t_command *)node->data;
	if (ft_process_redirections(shell, clean_tokens, cmd) != 0)
	{
		ft_free_ast(node);
		ft_free_token_list(clean_tokens);
		return (-1);
	}
	return (0);
}

t_ast_node	*ft_build_command_node(t_list *tokens, t_shell *shell)
{
	t_ast_node	*node;
	char		**args;
	int			word_count;
	t_list		*clean_tokens;

	if (ft_setup_command_tokens(tokens, &clean_tokens, &word_count) != 0)
		return (NULL);
	args = ft_extract_args_from_tokens(clean_tokens, word_count);
	if (!args)
	{
		ft_free_token_list(clean_tokens);
		return (NULL);
	}
	node = ft_create_node_with_cleanup(args, clean_tokens);
	if (!node)
		return (NULL);
	if (ft_process_command_redirections(shell, clean_tokens, node) != 0)
		return (NULL);
	ft_free_token_list(clean_tokens);
	return (node);
}
