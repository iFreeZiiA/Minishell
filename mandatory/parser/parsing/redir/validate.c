/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/06 22:40:57 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Valide syntaxe des redirections
 * @param tokens Liste de tokens
 * @return int 0 valide, -1 erreur syntaxe
 */
int	ft_val_redir_syntax(t_list *tokens)
{
	t_list	*current;
	t_list	*next;

	if (!tokens)
		return (-1);
	current = tokens;
	while (current)
	{
		if (ft_is_redir_tok(current->content.token))
		{
			next = current->next;
			if (!next)
				return (-1);
			if (ft_val_redir_order(current, next) != 0)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
/**
 * @brief Vérifie ordre des tokens redirection
 * @param redir_token Token de redirection
 * @param file_token Token fichier suivant
 * @return int 0 valide, -1 invalide
 */

int	ft_val_redir_order(t_list *redir_token, t_list *file_token)
{
	if (!redir_token || !redir_token->content.token)
		return (-1);
	if (!file_token || !file_token->content.token)
		return (-1);
	if (file_token->content.token->type != TOKEN_WORD)
		return (-1);
	if (ft_val_filename(file_token->content.token->value) != 0)
		return (-1);
	return (0);
}
/**
 * @brief Vérifie conflits entre redirections
 * @param redirs Liste des redirections
 * @return int 0 pas de conflit, -1 conflit
 */

int	ft_check_redir_conf(t_list *redirs)
{
	t_list	*current;
	int		output_count;

	if (!redirs)
		return (0);
	output_count = 0;
	current = redirs;
	while (current)
	{
		if (current->type == TYPE_REDIR)
		{
			if (ft_is_out_redir(current->content.redir))
				output_count++;
		}
		current = current->next;
	}
	if (output_count > 1)
		return (-1);
	return (0);
}
/**
 * @brief Valide nom de fichier
 * @param filename Nom à valider
 * @return int 0 valide, -1 invalide
 */

int	ft_val_filename(char *filename)
{
	int	i;

	if (!filename)
		return (-1);
	if (ft_strlen(filename) == 0)
		return (-1);
	if (filename[0] == '\0')
		return (-1);
	i = 0;
	while (filename[i])
	{
		if (filename[i] == '\0')
			return (-1);
		i++;
	}
	return (0);
}
