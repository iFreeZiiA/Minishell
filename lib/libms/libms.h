/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:39:27 by jjorda            #+#    #+#             */
/*   Updated: 2025/04/18 19:11:31 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMS_H
# define LIBMS_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "../struct_lib.h"
# include "../libft/libft.h"

char	*ft_getenv_value(t_shell *shell, const char *key);

/* --------------------------------- LIST --------------------------------- */

void	*ft_clean_node_tok(t_list *tok_curr);
t_list	*ft_lstnew_cmd(t_command *cmd);
t_list	*ft_lstnew_redir(t_redir *redir);
t_list	*ft_lstnew_tok(t_token *token);

#endif