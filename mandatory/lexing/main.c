/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:53:14 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/05 19:40:59 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	ft_end(t_shell *shell, int err)
{
	(void)err;
	free(shell->env);
}

void	ft_initiate(t_shell	*shell, char **env)
{
	shell->env = malloc(sizeof(t_env));
	shell->env->env = env;
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	ft_initiate(&shell, env);
	ft_get_token(argc, argv);
	ft_printerr("%s\n", shell.env->env[0]);
	ft_printerr("OK\n");
	ft_end(&shell, 0);
	return (0);
}