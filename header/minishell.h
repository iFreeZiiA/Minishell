/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:44:27 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 11:12:03 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include "../lib/libms/libms.h"
# include "struct.h"
# include "macro.h"
# include "deployment/setup.h"
# include "parser/lexing.h"
# include "parser/parsing.h"
# include "parser/logical.h"
# include "parser/wildcard.h"
# include "parser/parentheses.h"
# include "parser/redir.h"
# include "parser/ast.h"
# include "parser/heredoc.h"
# include "parser/validate.h"
# include "parser/integration.h"
# include "minishell_executor.h"
# include "signals.h"
# include "executor.h"
# include "deployment/cleanup.h"

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>

/* ********************************** MAIN UTILS *************************** */

int		ft_exit_check(char *input);
char	*ft_read_input(int is_interactive);
int		ft_validate_input(char *input, t_shell *shell, int is_interactive);
int		ft_handle_parse_error(t_shell *shell, int parse_result,
			int is_interactive);
int		ft_process_command(char *input, t_shell *shell, int is_interactive);
int		ft_handle_input(t_shell *shell, int is_interactive);

#endif