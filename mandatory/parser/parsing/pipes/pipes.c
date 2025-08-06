/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:00:25 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Extrait les tokens à gauche du pipe
 * 
 * @param tokens Liste de tokens
 * @param pipe_pos Position du token pipe
 * @return t_list* Liste des tokens de gauche
 */
t_list	*ft_extract_left_tokens(t_list *tokens, t_list *pipe_pos)
{
	t_list	*current;
	t_list	*left_tokens;
	t_list	*new_node;

	if (!tokens || !pipe_pos)
		return (NULL);
	left_tokens = NULL;
	current = tokens;
	while (current && current != pipe_pos)
	{
		new_node = ft_duplicate_token_node(current);
		if (!new_node)
			return (ft_lstleak(&left_tokens, ft_clean_node_tok));
		ft_add_token_to_list(&left_tokens, new_node);
		current = current->next;
	}
	return (left_tokens);
}
/**
 * @brief Duplique un nœud de token
 * 
 * @param original Nœud original
 * @return t_list* Nouveau nœud dupliqué
 */

t_list	*ft_duplicate_token_node(t_list *original)
{
	t_list		*new_node;
	t_token		*new_token;
	t_content	content;

	if (!original || !original->content.token)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = original->content.token->type;
	new_token->value = ft_strdup(original->content.token->value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	content.token = new_token;
	new_node = ft_lstnew_tok(new_token);
	return (new_node);
}
/**
 * @brief Ajoute un token à la fin d'une liste
 * 
 * @param list Pointeur vers la liste
 * @param new_node Nouveau nœud à ajouter
 */

void	ft_add_token_to_list(t_list **list, t_list *new_node)
{
	t_list	*current;

	if (!list || !new_node)
		return ;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_node;
	new_node->prev = current;
}
/**
 * @brief Compte le nombre de pipes dans la liste
 * 
 * @param tokens Liste de tokens
 * @return int Nombre de pipes
 */

int	ft_count_pipes(t_list *tokens)
{
	t_list	*current;
	int		count;

	if (!tokens)
		return (0);
	current = tokens;
	count = 0;
	while (current)
	{
		if (current->content.token->type == TOKEN_PIPE)
			count++;
		current = current->next;
	}
	return (count);
}
/**
 * @brief Vérifie si une liste contient uniquement des tokens valides
 * 
 * @param tokens Liste de tokens
 * @return int 1 si valide, 0 sinon
 */

int	ft_is_valid_command_sequence(t_list *tokens)
{
	t_list	*current;
	int		has_word;

	if (!tokens)
		return (0);
	current = tokens;
	has_word = 0;
	while (current)
	{
		if (current->content.token->type == TOKEN_WORD)
			has_word = 1;
		else if (current->content.token->type == TOKEN_PIPE)
			return (0);
		current = current->next;
	}
	return (has_word);
}
