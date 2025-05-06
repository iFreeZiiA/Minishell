/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:44:02 by jjorda            #+#    #+#             */
/*   Updated: 2025/05/06 19:19:17 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* -------------------------------- PARSING --------------------------------- */

/**
 * @brief Main parsing function to build the AST
 * 
 * @param shell The shell structure
 * @return int 0 on success, -1 on error
 */
int	ft_parsing(t_shell *shell);

/* ------------------------------- REDIRECTIONS ------------------------------ */

/**
 * @brief Parses a redirection token and creates a redirection structure
 * 
 * @param shell The shell structure
 * @param tok_curr Current token (redirection operator)
 * @return t_redir* New redirection structure, NULL on error
 */
t_redir	*parse_redirection(t_shell *shell, t_list *tok_curr);

/**
 * @brief Processes heredoc redirections
 * 
 * @param shell The shell structure
 * @param redir Redirection structure
 * @return int File descriptor for the heredoc, -1 on error
 */
int	process_heredoc(t_shell *shell, t_redir *redir);

/**
 * @brief Adds a redirection to a command
 * 
 * @param cmd Command to add redirection to
 * @param redir Redirection to add
 * @return int 0 on success, -1 on error
 */
int	add_redirection(t_command *cmd, t_redir *redir);

/**
 * @brief Collects all redirections for a command
 * 
 * @param shell The shell structure
 * @param start First token of the command
 * @param end Last token of the command
 * @param cmd Command to add redirections to
 * @return int Number of redirections added, -1 on error
 */
int	collect_redirections(t_shell *shell, t_list *start, t_list *end, t_command *cmd);

/* -------------------------------- AST UTILS ------------------------------- */

/**
 * @brief Creates an AST from the command list
 * 
 * @param cmd_list List of commands
 * @return t_ast_node* Root of the AST, NULL on error
 */
t_ast_node	*ft_create_ast(t_list *cmd_list);

/**
 * @brief Frees an AST
 * 
 * @param ast Root of the AST
 */
void	ft_free_ast(t_ast_node *ast);

/**
 * @brief Prints an AST for debugging purposes
 * 
 * @param ast Root of the AST
 * @param level Current level in the tree
 */
void	ft_print_ast(t_ast_node *ast, int level);

#endif