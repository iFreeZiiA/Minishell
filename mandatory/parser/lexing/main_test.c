/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:14:55 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 18:31:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../header/minishell.h"

/**
 * @brief Prints the token list for debugging purposes
 * 
 * @param tok_h Head of the token list
 */
void	ft_print_list(t_list *tok_h)
{
	t_list	*tok_curr;
	t_token	*tok;

	if (!tok_h)
		return;
	tok_curr = tok_h;
	while (tok_curr)
	{
		tok = tok_curr->content.token;
		if (!tok)
			break;
		ft_printerr("%s: %d\n", tok->value, tok->type);
		tok_curr = tok_curr->next;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	t_list	*head;
	int		res = 0;

	if (argc != 2)
	{
		ft_printerr("Please enter an argument to lexe...\n");
		return (0);
	}
	ft_setup(&shell, env);
	shell.current_line = argv[1];
	// ft_printerr("%s\n", );
	head = ft_lexing(&shell);
	if (!head)
		return (1);
	// shell.token = head;
	// ft_printerr("PING MAIN\n");
	if (res)
	{
		if (res == -1)
			ft_printerr("MALLOC ERRRO\n");
		if (res == -2)
			ft_printerr("WRITE ERROR\n");
		if (res == -3)
			ft_printerr("INVALID ARGUMENT\n");
		ft_printerr("%d\n", res);
		return (res);
	}
	// ft_printerr("PING main 0\n");
	// shell.env->env_vars = env;
	// ft_printerr("PING main 1%p\n", shell.token->content.token);
	// ft_printerr("tok_h: %s\n", shell.token->content.token->value);
	ft_print_list(shell.token);
	// ft_printerr("PING main 2\n");
	ft_lstfree_t(shell.token);
	// ft_printerr("PING\n");
	if (shell.env)
	{
		if (shell.env->local_env)
			free(shell.env->local_env);
		free(shell.env);
	}
	// ft_cleanup(&shell, 0);
	// ft_printerr("PING main 3\n");
	return (0);
}
