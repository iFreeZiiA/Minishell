/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enhanced.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 23:30:54 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

/**
 * @brief Détecte la présence d'opérateurs logiques dans les tokens
 * 
 * @param tokens Liste de tokens
 * @return bool true si opérateurs logiques détectés
 */
static bool	ft_has_logical_ops(t_list *tokens)
{
	t_list	*curr;
	t_token	*token;

	curr = tokens;
	while (curr)
	{
		token = curr->content.token;
		if (token->type == TOKEN_AND || token->type == TOKEN_OR)
			return (true);
		curr = curr->next;
	}
	return (false);
}

/**
 * @brief Détecte la présence de pipes dans les tokens
 * 
 * @param tokens Liste de tokens
 * @return bool true si pipes détectés
 */
static bool	ft_has_pipes(t_list *tokens)
{
	t_list	*curr;
	t_token	*token;

	curr = tokens;
	while (curr)
	{
		token = curr->content.token;
		if (token->type == TOKEN_PIPE)
			return (true);
		curr = curr->next;
	}
	return (false);
}

/**
 * @brief Détecte la présence de redirections dans les tokens
 * 
 * @param tokens Liste de tokens
 * @return bool true si redirections détectées
 */
static bool	ft_has_redirections(t_list *tokens)
{
	t_list	*curr;
	t_token	*token;

	curr = tokens;
	while (curr)
	{
		token = curr->content.token;
		if (ft_is_redir_tok(token))
			return (true);
		curr = curr->next;
	}
	return (false);
}

/**
 * @brief Traite les redirections et met à jour la commande
 * 
 * @param tokens Liste de tokens
 * @param shell Structure shell
 * @param cmd Structure commande à enrichir
 * @return int 0 succès, -1 erreur
 */
static int	ft_process_redirections(t_list *tokens, t_shell *shell,
		t_command *cmd)
{
	if (!ft_has_redirections(tokens))
		return (0);
	return (ft_parse_redir(shell, tokens, cmd));
}

/**
 * @brief Crée un nœud AST command enrichi avec redirections
 * 
 * @param tokens Liste de tokens
 * @param shell Structure shell
 * @return t_ast_node* Nœud AST de commande
 */
static t_ast_node	*ft_create_enhanced_command(t_list *tokens, t_shell *shell)
{
	t_ast_node	*node;
	t_command	*cmd;
	char		**args;
	int			word_count;

	word_count = ft_count_word_tokens(tokens);
	if (word_count == 0)
		return (NULL);
	args = ft_extract_command_args(tokens, word_count);
	if (!args)
		return (NULL);
	node = ft_new_simple_cmd(args);
	if (!node)
		return (NULL);
	cmd = (t_command *)node->data;
	if (ft_process_redirections(tokens, shell, cmd) != 0)
	{
		ft_free_ast(node);
		return (NULL);
	}
	return (node);
}

/**
 * @brief Parser principal amélioré utilisant toutes les fonctions
 * Remplace l'ancienne version de ft_parser pour être plus complet
 * 
 * @param tokens Liste de tokens du lexer
 * @param shell Structure shell
 * @return t_ast_node* Noeud racine de l'AST ou NULL en cas d'erreur
 */
t_ast_node	*ft_enhanced_parser(t_list *tokens, t_shell *shell)
{
	t_list	*last_token;

	if (ft_validate_token_list(tokens) != 0)
		return (NULL);
	
	// Priorité 1: Opérateurs logiques (plus haute précédence)
	if (ft_has_logical_ops(tokens))
	{
		last_token = ft_lstlast(tokens);
		if (!last_token)
			return (NULL);
		return (ft_parse_logical_expression(shell, tokens, last_token));
	}
	
	// Priorité 2: Si des pipes sont détectés, utiliser le parser de pipes
	if (ft_has_pipes(tokens))
		return (ft_parse_pipe_expression(tokens, shell));
	
	// Priorité 3: Sinon, traiter comme une commande simple
	return (ft_create_enhanced_command(tokens, shell));
}
