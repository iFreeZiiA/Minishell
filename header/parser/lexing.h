/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:43:30 by jjorda            #+#    #+#             */
/*   Updated: 2025/07/27 18:43:31 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

/* ---------------------------------- ERR ----------------------------------- */

/**
 * @brief Sets a status code and returns NULL
 * 
 * @param p_status Pointer to status variable
 * @param status Status code to set
 * @return void* Always NULL
 */
void			*ft_tok_err(int *p_status, int status);

/**
 * @brief Frees an entire token list
 * 
 * @param tok_h Head of the token list
 * @return void* Always NULL
 */
void			*ft_lstfree_t(t_list *tok_h);

/**
 * @brief Creates a new token error
 * 
 * @param token The token to check
 * @param ret Return value
 * @return int The return value
 */
int				ft_new_token_err(t_token *token, int ret);

/**
 * @brief Performs basic syntax validation on the token list
 * 
 * @param shell The shell structure
 * @return int 0 on success, negative value on error
 */
int				ft_lexer_checker(t_shell *shell);

/* --------------------------------- UTILS ---------------------------------- */

// Fonctions utilitaires de base
int				ft_is_operator(char c);
int				ft_is_space(char c);
void			ft_skip_spaces(char *line, int *i);
t_token			*ft_create_token(char *value, t_token_type type);

// Fonctions d'extraction
char			*ft_extract_word(char *line, int *i);
char			*ft_extract_operator(char *line, int *i);
t_token_type	ft_get_operator_type(char *op);

// Fonctions de tokenisation
int				ft_add_token(t_list **tokens, char *value, t_token_type type);
t_list			*ft_lexing_simple(t_shell *shell);

// Fonctions d'expansion variables
int				ft_env_var_match(char *env_line, char *var_name);
char			*ft_expand_var(char *str, char **envp, t_shell *shell);
char			*ft_get_var_value(char *var_name, char **envp, t_shell *shell);
char			*ft_expand_string(char *str, char **envp, t_shell *shell);
char			*ft_extract_var_name(char *str, int pos);
char			*ft_replace_var(char *str, int pos, int var_len, char *replacement);

// Fonctions d'expansion finale
char			*ft_remove_quotes(char *str);
void			ft_expand_tokens(t_list *tokens, char **envp, t_shell *shell);

/* --------------------------------- LEXING --------------------------------- */

/**
 * @brief Main lexing function - point d'entrée principal
 * 
 * @param shell The shell structure
 * @return t_list* Head of the token list, NULL on error
 */
t_list			*ft_lexing_new(t_shell *shell);

#endif