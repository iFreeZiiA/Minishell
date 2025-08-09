/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:45:58 by jjorda            #+#    #+#             */
/*   Updated: 2025/08/09 16:35:58 by jjorda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_list	t_list;

/* --------------------------------- LEXING --------------------------------- */

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OR,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_STATUS,
	TOKEN_ERROR,
	TOKEN_PIPE,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_IN,
	TOKEN_VAR,
	TOKEN_QUOTE,
	TOKEN_DQUOTE,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_WILDCARD,
	TOKEN_ASSIGN,
	TOKEN_EOF,
	TOKEN_BSLASH,
	TOKEN_SPACE
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

/* -------------------------------- PARSING --------------------------------- */

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_REDIR,
	NODE_GROUP
}	t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	void				*data;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

/* --------------------------------- REDIR ---------------------------------- */

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC,
	REDIR_APPEND
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	int				fd;
}	t_redir;

/* -------------------------------- COMMAND --------------------------------- */

typedef struct s_command
{
	char	**args;
	t_list	*redirs;
}	t_command;

/* ---------------------------------- ENV ----------------------------------- */

typedef struct s_env
{
	char	**env_vars;
	char	**local_env;
	int		last_exit_code;
}	t_env;

/* --------------------------------- SHELL ---------------------------------- */

typedef struct s_shell
{
	t_env		*env;
	char		*current_line;
	int			signal_received;
	char		*prompt;
	t_list		*token;
	t_ast_node	*ast;
	int			last_exit_code;
}	t_shell;

/* ---------------------------------- LIST ---------------------------------- */

typedef enum e_content_type
{
	TYPE_REDIR = 1,
	TYPE_TOKEN,
	TYPE_COMMAND,
	TYPE_GENERIC
}	t_content_type;

typedef union u_content
{
	t_redir		*redir;
	t_token		*token;
	t_command	*cmd;
	void		*generic;
}	t_content;

typedef struct s_list
{
	t_content_type	type;
	t_content		content;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

typedef struct s_match_data
{
	char	**matches;
	int		count;
	int		capacity;
}	t_match_data;

#endif