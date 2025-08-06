/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enhanced.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 18:01:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

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
		if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT ||
			token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
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
static int	ft_process_redirections(t_list *tokens, t_shell *shell, t_command *cmd)
{
	if (!ft_has_redirections(tokens))
		return (0);
	return (ft_parse_redirections(shell, tokens, cmd));
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
	node = ft_create_simple_command_node(args);
	if (!node)
		return (NULL);
	cmd = (t_command *)node->data;
	if (ft_process_redirections(tokens, shell, cmd) != 0)
	{
		ft_free_ast_node(node);
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
	// printf("DEBUG: ft_enhanced_parser called\n");
	if (ft_validate_token_list(tokens) != 0)
	{
		// printf("DEBUG: Token validation failed\n");
		return (NULL);
	}
	// printf("DEBUG: Token validation passed, creating enhanced command\n");
	return (ft_create_enhanced_command(tokens, shell));
}
