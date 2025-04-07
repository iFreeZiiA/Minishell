/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_executor.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alearroy <alearroy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:09:38 by alearroy          #+#    #+#             */
/*   Updated: 2025/04/07 19:05:00 by alearroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXECUTOR_H
# define MINISHELL_EXECUTOR_H

# include <string.h>

int		run_builtin(char **args, char ***env);
char	**ft_envdup(char **envp);
int		get_env_index(char **env, const char *key);
int		builtin_echo(char **args);
int		builtin_cd(char **args, char ***env);
int		builtin_pwd(void);
int		builtin_env(char **env);
int		builtin_exit(char **args);

#endif