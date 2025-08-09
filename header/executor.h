/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:41:32 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/09 16:14:47 by jjorda           ###   ########.fr       */
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
// int		handle_heredoc(char *delimiter);
int		is_builtin(char *cmd);
void	ft_expand_command_args(t_command *cmd, t_env *env);
char	*ft_handle_export_arg(char *arg);
int		ft_should_skip_expansion(char *cmd_name, int arg_index, char *arg);
void	ft_process_special_arg(char **arg);
void	ft_process_normal_arg(char **arg, char **envp, t_shell *shell);
int		ft_create_pipe_and_fork(t_list *cmd_h, pid_t *pids, int *prev, int i);
int		ft_execute_pipe_loop(t_list *cmd_h, pid_t *pids, t_env *env);
void	ft_setup_child_fds(int in, int out);
void	ft_execute_child_command(t_command *cmd, t_env *env);

#endif
