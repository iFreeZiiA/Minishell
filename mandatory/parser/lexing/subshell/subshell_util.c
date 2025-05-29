/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:30:00 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/23 23:16:40 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../header/minishell.h"

/**
 * @brief Creates a subshell environment copy
 * 
 * @param parent_env Parent environment
 * @return t_env* New environment for subshell
 */
static t_env	*ft_create_subshell_env(t_env *parent_env)
{
	t_env	*sub_env;

	if (!parent_env)
		return (NULL);
	
	sub_env = (t_env *)malloc(sizeof(t_env));
	if (!sub_env)
		return (NULL);
	
	// For simplicity, just copy the pointers
	// In a real implementation, you'd want to duplicate the arrays
	sub_env->env_vars = parent_env->env_vars;
	sub_env->local_env = parent_env->local_env;
	sub_env->last_exit_code = parent_env->last_exit_code;
	
	return (sub_env);
}

/**
 * @brief Frees a subshell environment
 * 
 * @param env Environment to free
 */
static void	ft_free_subshell_env(t_env *env)
{
	if (!env)
		return;
	
	// Don't free the arrays since they're shared with parent
	free(env);
}

/**
 * @brief Executes commands in a subshell (fork)
 * 
 * @param content AST node with the subshell content
 * @param parent_env Parent environment
 * @return int Exit code of the subshell
 */
int	ft_execute_subshell(t_ast_node *content, t_env *parent_env)
{
	pid_t	pid;
	int		status;
	t_env	*sub_env;
	int		exit_code;

	if (!content || !parent_env)
		return (1);
	
	pid = fork();
	if (pid == -1)
	{
		ft_printerr("minishell: fork failed\n");
		return (1);
	}
	
	if (pid == 0)
	{
		// Child process (subshell)
		sub_env = ft_create_subshell_env(parent_env);
		if (!sub_env)
			exit(1);
		
		// Execute the content in the subshell
		exit_code = ft_execute_logical(content, sub_env);
		
		ft_free_subshell_env(sub_env);
		exit(exit_code);
	}
	
	// Parent process
	waitpid(pid, &status, 0);
	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	
	return (1);
}
