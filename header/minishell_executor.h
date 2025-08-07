/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_executor.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:09:38 by alearroy          #+#    #+#             */
/*   Updated: 2025/08/07 22:39:22 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXECUTOR_H
# define MINISHELL_EXECUTOR_H

# include <string.h>

int		run_builtin(char **args, char ***env);
char	**ft_envdup(char **envp);
int		get_env_index(char **env, const char *key);
int		update_env_var(char ***env, const char *key, const char *value);
int		update_env_var(char ***env, const char *key, const char *value);
int		builtin_echo(char **args);
int		builtin_cd(char **args, t_shell *shell);
int		builtin_cd_direct(char **args, char ***env);
int		builtin_pwd(void);
int		builtin_env(char **env);
int		builtin_unset(char **args, char ***env);
int		builtin_exit(char **args);
int		builtin_export(char **args, char ***env);

#endif