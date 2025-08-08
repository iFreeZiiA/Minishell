# ************************************************************************** #
#              # Validation
SRC_PARSING_VALIDATE = mandatory/parser/parsing/validate/validate.c \
	mandatory/parser/parsing/validate/utils.c \
	mandatory/parser/parsing/validate/err.c \
	mandatory/parser/parsing/validate/advanced.c \
	mandatory/parser/parsing/validate/adv_utils.c \
	mandatory/parser/parsing/validate/validate_core.c                                                      #
#                                                        :::      ::::::::   #
#   Sources_new.mk                                     :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/08/06 00:00:00 by jjorda            #+#    #+#             #
#   Updated: 2025/08/06 00:00:00 by jjorda           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# Sources propres basées sur les fichiers réellement compilés et fonctionnels

# ********************************* CORE *********************************** #

SRC_MAIN = mandatory/main.c

# ******************************** BUILTIN ********************************** #

SRC_BUILTIN = mandatory/builtin/builtin_cd_utils.c \
	mandatory/builtin/builtin_pwd.c \
	mandatory/builtin/run_builtin.c \
	mandatory/builtin/builtin_unset.c \
	mandatory/builtin/builtin_export.c \
	mandatory/builtin/builtin_env.c \
	mandatory/builtin/builtin_exit.c \
	mandatory/builtin/builtin_echo.c \
	mandatory/builtin/builtin_cd.c

# ******************************* EXECUTOR ********************************* #

SRC_EXECUTOR = mandatory/executor/executor.c \
	mandatory/executor/ast_to_command_list.c \
	mandatory/executor/get_path.c

# ******************************* SIGNAL *********************************** #

SRC_SIGNAL = mandatory/signal/signal.c

# ****************************** DEPLOYMENT ******************************** #

SRC_DEPLOYMENT = mandatory/deployment/setup/setup.c \
	mandatory/deployment/setup/environment.c \
	mandatory/deployment/cleanup/cleanup.c

# ******************************* PARSER *********************************** #
# Parser de base
SRC_PARSER_BASE = mandatory/parser/enhanced.c \
	mandatory/parser/parser.c

# ******************************** LEXING *********************************** #

SRC_LEXING = mandatory/parser/lexing/err.c \
	mandatory/parser/lexing/utils.c \
	mandatory/parser/lexing/extract.c \
	mandatory/parser/lexing/tokenize.c \
	mandatory/parser/lexing/variables.c \
	mandatory/parser/lexing/parse_var.c \
	mandatory/parser/lexing/expand.c

# ******************************** PARSING ********************************** #
# AST
SRC_PARSING_AST = mandatory/parser/parsing/ast/create.c \
	mandatory/parser/parsing/ast/free.c \
	mandatory/parser/parsing/ast/utils.c \
	mandatory/parser/parsing/ast/cmd_build.c \
	mandatory/parser/parsing/ast/cmd_build_utils.c

# Core parser
SRC_PARSING_CORE = mandatory/parser/parsing/parser_core.c

# Opérateurs logiques
SRC_PARSING_LOGICAL = mandatory/parser/parsing/logical/logical.c \
	mandatory/parser/parsing/logical/precedence.c \
	mandatory/parser/parsing/logical/utils.c

# Pipes
SRC_PARSING_PIPES = mandatory/parser/parsing/pipes/operators.c \
	mandatory/parser/parsing/pipes/pipes.c

# Redirections
SRC_PARSING_REDIR = mandatory/parser/parsing/redir/input.c \
	mandatory/parser/parsing/redir/output.c \
	mandatory/parser/parsing/redir/redir.c \
	mandatory/parser/parsing/redir/utils.c \
	mandatory/parser/parsing/redir/validate.c

# Utilitaires
SRC_PARSING_UTILS = mandatory/parser/parsing/utils/token_utils.c \
	mandatory/parser/parsing/utils/token_list.c

# Validation
SRC_PARSING_VALIDATE = mandatory/parser/parsing/validate/validate.c \
	mandatory/parser/parsing/validate/utils.c \
	mandatory/parser/parsing/validate/err.c \
	mandatory/parser/parsing/validate/advanced.c \
	mandatory/parser/parsing/validate/adv_utils.c \
	mandatory/parser/parsing/validate/validate_core.c \
	mandatory/parser/parsing/validate/validate_substitutions.c

# Heredoc
SRC_PARSING_HEREDOC = mandatory/parser/heredoc/heredoc.c \
	mandatory/parser/heredoc/heredoc_process.c

# Wildcard
SRC_PARSING_WILDCARD = mandatory/parser/wildcard/wildcard.c \
	mandatory/parser/wildcard/wildcard_match.c \
	mandatory/parser/wildcard/wildcard_files.c \
	mandatory/parser/wildcard/wildcard_utils.c

# Parentheses
SRC_PARSING_PARENTHESES = mandatory/parser/parentheses/parentheses.c \
	mandatory/parser/parentheses/parentheses_validate.c \
	mandatory/parser/parentheses/parentheses_find.c \
	mandatory/parser/parentheses/parentheses_parse.c \
	mandatory/parser/parentheses/parentheses_logic.c

# Regroupement parsing
SRC_PARSING = $(SRC_PARSING_AST) $(SRC_PARSING_CORE) $(SRC_PARSING_LOGICAL) $(SRC_PARSING_PIPES) \
	$(SRC_PARSING_REDIR) $(SRC_PARSING_UTILS) $(SRC_PARSING_VALIDATE) $(SRC_PARSING_HEREDOC) $(SRC_PARSING_WILDCARD) $(SRC_PARSING_PARENTHESES)

# ****************************** SOURCES FINALES *************************** #

SRC = $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_SIGNAL) $(SRC_EXECUTOR) \
	$(SRC_DEPLOYMENT) $(SRC_PARSER_BASE) $(SRC_LEXING) $(SRC_PARSING)

# ********************************* BONUS *********************************** #

BONUS_SOURCES = bonus/main.c

# ************************************ LIB *********************************** #

LIB			= libs
DIR_LIB		= ./lib
LIBS		= -L$(DIR_LIBFT) -lft -L$(DIR_LIBMS) -lms -lreadline

DIR_LIBFT	= $(DIR_LIB)/libft
LIBFT		= $(DIR_LIBFT)/libft.a

DIR_LIBMS	= $(DIR_LIB)/libms
LIBMS		= $(DIR_LIBMS)/libms.a

# ********************************* OBJECTS ********************************** #

DIR_OBJ		= obj
DIR			= $(DIR_OBJ)/
OBJ			= $(patsubst %.c, $(DIR)%.o, $(SRC))
