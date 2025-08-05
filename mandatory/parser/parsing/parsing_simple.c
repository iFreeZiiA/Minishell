/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/02 12:33:52 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"




static int	ft_parse_simple_redirections(t_list *tokens, t_command *cmd)
{
	t_list	*current;
	t_token	*token;
	
	current = tokens;
	while (current)
	{
		if (current->type == TYPE_TOKEN)
		{
			token = current->content.token;
			if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_REDIR_IN
				|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
			{
				// Pour Phase 5, on accepte mais ignore les redirections
				// Elles seront implémentées en Phase 6
				(void)cmd;
				// Continuer le parsing au lieu de retourner erreur
			}
		}
		current = current->next;
	}
	return (1);
}

t_ast_node	*ft_parse_simple_command(t_list *tokens)
{
	t_command	*cmd;
	t_ast_node	*node;
	char		**args;
	int			count;
	
	if (!tokens)
		return (NULL);
	count = ft_count_word_tokens(tokens);
	if (count == 0)
		return (NULL);
	args = ft_extract_command_args(tokens, count);
	if (!args)
		return (NULL);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		ft_free_arr(args);
		return (NULL);
	}
	cmd->args = args;
	cmd->redirs = NULL;
	
	// Parser les redirections simples
	if (!ft_parse_simple_redirections(tokens, cmd))
	{
		free(cmd);
		ft_free_arr(args);
		return (NULL);
	}
	
	node = malloc(sizeof(t_ast_node));
	if (!node)
	{
		free(cmd);
		ft_free_arr(args);
		return (NULL);
	}
	node->type = NODE_COMMAND;
	node->data = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node	*ft_create_pipe_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;
	
	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->data = NULL;
	node->left = left;
	node->right = right;
	return (node);
}


t_ast_node	*ft_parse_with_pipes(t_list *tokens)
{
	// Phase 5: Parser de base - pas de pipes complexes encore
	// On traite tout comme une commande simple pour la stabilité
	if (!tokens)
		return (NULL);
	return (ft_parse_simple_command(tokens));
}

static int	ft_validate_ast(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		t_command *cmd = (t_command *)node->data;
		if (!cmd || !cmd->args || !cmd->args[0])
			return (0);
	}
	else if (node->type == NODE_PIPE)
	{
		if (!node->left || !node->right)
			return (0);
		if (!ft_validate_ast(node->left) || !ft_validate_ast(node->right))
			return (0);
	}
	return (1);
}

t_ast_node	*ft_parser_new(t_list *tokens, t_shell *shell)
{
	t_ast_node	*ast;
	
	(void)shell;
	if (!tokens)
		return (NULL);
	
	ast = ft_parse_with_pipes(tokens);
	if (!ast)
		return (NULL);
	
	// Validation de l'AST créé
	if (!ft_validate_ast(ast))
	{
		ft_free_ast_node(ast);
		return (NULL);
	}
	
	return (ast);
}
