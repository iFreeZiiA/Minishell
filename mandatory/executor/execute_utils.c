/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:00:00 by jjorda            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/08/09 13:18:27 by jjorda           ###   ########.fr       */
=======
/*   Updated: 2025/08/09 15:00:00 by jjorda           ###   ########.fr       */
>>>>>>> d461779 (Norminette half done)
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static void	ft_create_temp_shell(t_shell *temp_shell, t_env *env)
{
	temp_shell->env = env;
}

<<<<<<< HEAD
static char	*ft_handle_export_arg(char *arg)
{
	char	*equals_pos;
	char	*var_name;
	char	*var_value;
	char	*result;
	char	*marker_pos;
	
	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
		return (ft_strdup(arg));
	
	// Séparer le nom de variable et la valeur
	var_name = ft_substr(arg, 0, equals_pos - arg);
	var_value = equals_pos + 1;
	
	// Chercher le marqueur \x01 dans la valeur
	marker_pos = ft_strchr(var_value, '\x01');
	if (marker_pos)
	{
		// Construire le résultat sans expansion: VAR=valeur_littérale
		result = ft_strjoin(var_name, "=");
		free(var_name);
		var_name = result;
		result = ft_strjoin(var_name, marker_pos + 1); // +1 pour ignorer le marqueur \x01
		free(var_name);
		return (result);
	}
	else
	{
		// Traitement normal (avec expansion possible)
		free(var_name);
		return (ft_strdup(arg));
	}
}

static int	ft_should_skip_expansion(char *cmd_name, int arg_index, char *arg)
{
	// Pour export, on ne doit pas expandre les valeurs qui étaient dans des single quotes
	if (!ft_strcmp(cmd_name, "export") && arg_index > 0)
	{
		char *equals_pos = ft_strchr(arg, '=');
		if (equals_pos)
		{
			// Vérifier si la valeur après = contient le marqueur de single quote
			if (ft_strchr(equals_pos + 1, '\x01'))
				return (1);
		}
	}
	return (0);
}

=======
>>>>>>> d461779 (Norminette half done)
void	ft_expand_command_args(t_command *cmd, t_env *env)
{
	t_shell	temp_shell;
	int		i;
	char	*expanded;
	char	*cleaned;

	if (!cmd || !cmd->args || !env)
		return ;
	ft_create_temp_shell(&temp_shell, env);
	i = 0;
	while (cmd->args[i])
	{
<<<<<<< HEAD
		// Vérifier si on doit éviter l'expansion pour cet argument
		if (ft_should_skip_expansion(cmd->args[0], i, cmd->args[i]))
		{
			// Pour les arguments export avec single quotes, on traite spécialement
			cleaned = ft_handle_export_arg(cmd->args[i]);
			free(cmd->args[i]);
			cmd->args[i] = cleaned;
		}
		else
		{
			expanded = ft_exp_string(cmd->args[i], env->env_vars, &temp_shell);
			if (expanded)
			{
				cleaned = ft_rm_quotes(expanded);
				free(cmd->args[i]);
				free(expanded);
				cmd->args[i] = cleaned;
			}
		}
=======
		expanded = ft_exp_string(cmd->args[i], env->env_vars, &temp_shell);
		if (expanded)
		{
			cleaned = ft_rm_quotes(expanded);
			free(cmd->args[i]);
			free(expanded);
			cmd->args[i] = cleaned;
		}
>>>>>>> d461779 (Norminette half done)
		i++;
	}
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}
