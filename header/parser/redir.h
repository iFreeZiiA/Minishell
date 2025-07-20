/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:20:08 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/20 16:40:25 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Fonctions principales
int         ft_parse_redirections(t_shell *shell, t_list *tokens, t_command *cmd);
redir_type  ft_get_redirection_type(t_token *token);
t_list      *ft_find_next_redirection(t_list *tokens);
t_redir     *ft_create_redirection(redir_type type, char *filename);

// Parsing d'entrée
int         ft_parse_input_redirection(t_list *tokens, t_command *cmd);
int         ft_parse_heredoc_redirection(t_list *tokens, t_command *cmd);
int         ft_validate_input_file(char *filename);
char        *ft_extract_heredoc_delimiter(t_list *tokens);

// Parsing de sortie
int         ft_parse_output_redirection(t_list *tokens, t_command *cmd);
int         ft_parse_append_redirection(t_list *tokens, t_command *cmd);
int         ft_validate_output_file(char *filename);
int         ft_check_write_permissions(char *filename);

// Validation
int         ft_validate_redirection_syntax(t_list *tokens);
int         ft_validate_redirection_order(t_list *redir_token, t_list *file_token);
int         ft_check_redirection_conflicts(t_list *redirs);
int         ft_validate_filename(char *filename);

// Utilitaires
int         ft_count_redirections(t_list *tokens);
char        *ft_extract_redirection_file(t_list *redir_token);
int         ft_is_redirection_token(t_token *token);
int         ft_process_single_redirection(t_list *redir_token, t_command *cmd);
char        *ft_extract_directory(char *filepath);
int         ft_is_input_redirection(t_redir *redir);
int         ft_is_output_redirection(t_redir *redir);
void	ft_free_redirection_list(t_list *redirs);
