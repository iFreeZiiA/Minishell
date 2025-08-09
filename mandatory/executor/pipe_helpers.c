/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:11:02 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:42:17 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

void	close_pipe_and_update(int *prev, int *pipe_fd)
{
	close(pipe_fd[1]);
	*prev = pipe_fd[0];
}

void	wait_all_pids(pid_t *pids, int count, t_env *env)
{
	int	i;
	int	status;
	int	sig;

<<<<<<< HEAD
	// Pendant que le parent attend, ignorer SIGINT et SIGQUIT
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
=======
>>>>>>> d461779 (Norminette half done)
	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
		{
<<<<<<< HEAD
			if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				if (sig == SIGQUIT)
					write(2, "Quit (core dumped)\n", 19);
				env->last_exit_code = 128 + sig;
			}
			else if (WIFEXITED(status))
=======
			if (WIFEXITED(status))
>>>>>>> d461779 (Norminette half done)
				env->last_exit_code = WEXITSTATUS(status);
		}
		i++;
	}
<<<<<<< HEAD
	
	// Restaurer la gestion interactive des signaux
	restore_interactive_signals();
=======
>>>>>>> d461779 (Norminette half done)
}

int	ft_create_pipe_and_fork(t_list *cmd_h, pid_t *pids, int *prev, int i)
{
	int		pipe_fd[2];

	if (cmd_h->next && pipe(pipe_fd) == -1)
		return (-1);
	pids[i] = fork();
	if (pids[i] == 0)
	{
		if (cmd_h->next)
			return (pipe_fd[1]);
		return (-1);
	}
	if (*prev != -1)
		close(*prev);
	if (cmd_h->next)
		close_pipe_and_update(prev, pipe_fd);
	return (0);
}

int	ft_execute_pipe_loop(t_list *cmd_h, pid_t *pids, t_env *env)
{
	int		prev;
	int		i;
	int		out;

	i = 0;
	prev = -1;
	while (cmd_h)
	{
		out = ft_create_pipe_and_fork(cmd_h, pids, &prev, i);
		if (out == -1 && pids[i] != 0)
			return (-1);
		if (pids[i] == 0)
			child_process(cmd_h, prev, out, env);
		cmd_h = cmd_h->next;
		i++;
	}
	return (i);
}
