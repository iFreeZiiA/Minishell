/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:20:08 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 11:16:20 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Fonctions principales
int				ft_parse_redir(t_shell *shell, t_list *tokens, t_command *cmd);
t_redir_type	ft_get_t_redir_type(t_token *token);
t_list			*ft_find_next_redir(t_list *tokens);
t_redir			*ft_new_redir(t_redir_type type, char *filename);

// Parsing d	'entrée
int				ft_parse_in_redir(t_list *tokens, t_command *cmd);
int				ft_parse_heredoc(t_list *tokens, t_command *cmd);
int				ft_val_in_file(char *filename);
char			*ft_ext_heredoc_delim(t_list *tokens);

// Parsing d	e sortie
int				ft_parse_out_redir(t_list *tokens, t_command *cmd);
int				ft_parse_append(t_list *tokens, t_command *cmd);
int				ft_val_out_file(char *filename);
int				ft_check_write_perm(char *filename);

// Validatio	n
int				ft_val_redir_syntax(t_list *tokens);
int				ft_val_redir_order(t_list *redir_token, t_list *file_token);
int				ft_check_redir_conf(t_list *redirs);
int				ft_val_filename(char *filename);

// Utilitair	es
char			*ft_ext_redir_file(t_list *redir_token);
int				ft_is_redir_tok(t_token *token);
int				ft_proc_single_redir(t_list *redir_token, t_command *cmd);
int				ft_is_out_redir(t_redir *redir);
