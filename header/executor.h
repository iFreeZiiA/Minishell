/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:41:32 by alearroy          #+#    #+#             */
/*   Updated: 2025/06/12 18:27:15 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

int		execute_command(t_command *cmd, t_env *env);
char	*get_path(char *cmd, char **envp);
t_list	*ast_to_command_list(t_ast_node *ast);
int		apply_redirections(t_list *redirs);
int		execute_pipe(t_list *cmd_h, t_env *env);
int		executor_from_ast(t_ast_node *ast, t_env *env);
void	close_pipe_and_update(int *prev, int *pipe_fd);
void	wait_all_pids(pid_t *pids, int count, t_env *env);
void	child_process(t_list *cmd_l, int in, int out, t_env *env);
void	free_command_list(t_list *cmds);

#endif
