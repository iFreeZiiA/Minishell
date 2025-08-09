/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:00:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Trouve le premier token pipe dans la liste
 * 
 * @param tokens Liste de tokens
 * @return t_list* Token pipe trouvé ou NULL
 */
t_list	*ft_find_pipe_token(t_list *tokens)
{
	t_list	*current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current)
	{
		if (current->content.token->type == TOKEN_PIPE)
			return (current);
		current = current->next;
	}
	return (NULL);
}
/**
 * @brief Crée un nœud AST de type pipe
 * 
 * @param left Commande de gauche
 * @param right Commande de droite
 * @return t_ast_node* Nouveau nœud pipe
 */

t_ast_node	*ft_create_pipe_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	if (!left || !right)
		return (NULL);
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->data = NULL;
	node->left = left;
	node->right = right;
	return (node);
}
/**
 * @brief Parse une expression avec pipe
 * 
 * @param tokens Liste de tokens
 * @param shell Structure shell
 * @return t_ast_node* Nœud AST pipe
 */

t_ast_node	*ft_parse_pipe_expression(t_list *tokens, t_shell *shell)
{
	t_list		*pipe_pos;
	t_list		*left_tokens;
	t_list		*right_tokens;
	t_ast_node	*left_node;
	t_ast_node	*right_node;

	pipe_pos = ft_find_pipe_token(tokens);
	if (!pipe_pos)
		return (ft_parser(tokens, shell));
	left_tokens = ft_extract_left_tokens(tokens, pipe_pos);
	right_tokens = pipe_pos->next;
	if (!left_tokens || !right_tokens)
		return (NULL);
	left_node = ft_parser(left_tokens, shell);
	right_node = ft_parse_pipe_expression(right_tokens, shell);
	if (!left_node || !right_node)
		return (NULL);
	return (ft_create_pipe_node(left_node, right_node));
}
/**
 * @brief Valide la syntaxe des pipes
 * 
 * @param tokens Liste de tokens
 * @return int 0 si valide, -1 sinon
 */

int	ft_validate_pipe_syntax(t_list *tokens)
{
	t_list	*current;
	t_list	*prev;

	if (!tokens)
		return (-1);
	current = tokens;
	prev = NULL;
	while (current)
	{
		if (current->content.token->type == TOKEN_PIPE)
		{
			if (!prev || !current->next)
				return (-1);
			if (prev->content.token->type != TOKEN_WORD)
				return (-1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
