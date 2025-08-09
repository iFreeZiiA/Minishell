/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:24:19 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

char	**ft_ext_args_toks(t_list *clean_tokens, int word_count)
{
	char	**args;

	args = ft_extract_command_args_until_pipe(clean_tokens, word_count);
	return (args);
}

t_ast_node	*ft_new_cmd_redir(t_list *tokens, t_shell *shell)
{
	return (ft_build_cmd_node(tokens, shell));
}
