/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:55:15 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Vérifie si un token est un opérateur
 * 
 * @param token Token à vérifier
 * @return int 1 si opérateur, 0 sinon
 */
int	ft_is_op_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR);
}

/**
 * @brief Vérifie une séquence invalide de tokens
 * 
 * @param prev Token précédent
 * @param current Token actuel
 * @return int 1 si séquence invalide, 0 sinon
 */
int	ft_check_inv_tok_seq(t_token *prev, t_token *current)
{
	if (!current)
		return (0);
	if (!prev)
	{
		if (ft_is_op_token(current))
			return (1);
		return (0);
	}
	if (ft_is_op_token(prev) && ft_is_op_token(current))
		return (1);
	if (ft_is_redir_tok(prev) && current->type != TOKEN_WORD)
		return (1);
	return (0);
}

/**
 * @brief Vérifie s'il y a une commande après un pipe
 * 
 * @param pipe_node Nœud contenant le pipe
 * @return int 1 si commande trouvée, 0 sinon
 */
int	ft_has_cmd_after_pipe(t_list *pipe_node)
{
	t_list	*current;
	t_token	*token;

	if (!pipe_node || !pipe_node->next)
		return (0);
	current = pipe_node->next;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_WORD)
			return (1);
		if (ft_is_op_token(token))
			return (0);
		current = current->next;
	}
	return (0);
}

/**
 * @brief Vérifie s'il y a une commande après un opérateur logique
 * 
 * @param logical_node Nœud contenant l'opérateur logique
 * @return int 1 si commande trouvée, 0 sinon
 */
int	ft_has_cmd_after_log(t_list *logical_node)
{
	t_list	*current;
	t_token	*token;

	if (!logical_node || !logical_node->next)
		return (0);
	current = logical_node->next;
	while (current)
	{
		token = (t_token *)current->content.token;
		if (token->type == TOKEN_WORD)
			return (1);
		if (ft_is_op_token(token))
			return (0);
		current = current->next;
	}
	return (0);
}

void	ft_print_heredoc(t_token *next_token)
{
	if (next_token)
		ft_printerr("bash: syntax error near unexpected token `%s'\n",
			next_token->value);
	else
		ft_printerr("bash: syntax error near unexpected token `newline'\n");
}
