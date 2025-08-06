# Configuration ultra-minimal pour tester SEULEMENT executor 

# ********************************* SOURCES ********************************** #

SRC_MAIN = mandatory/main.c

SRC_BUILTIN = mandatory/builtin/builtin_cd.c \
	mandatory/builtin/builtin_cd_utils.c \
	mandatory/builtin/builtin_echo.c \
	mandatory/builtin/builtin_env.c \
	mandatory/builtin/builtin_exit.c \
	mandatory/builtin/builtin_export.c \
	mandatory/builtin/builtin_pwd.c \
	mandatory/builtin/builtin_unset.c \
	mandatory/builtin/run_builtin.c

SRC_SIGNAL = mandatory/signal/signal.c

SRC_EXECUTOR = mandatory/executor/ast_to_command_list.c \
	mandatory/executor/executor.c \
	mandatory/executor/get_path.c

SRC_DEPLOYMENT = mandatory/deployment/setup/setup.c \
	mandatory/deployment/cleanup/cleanup.c

SRC_PARSER_BASE = mandatory/parser/parser.c \
	mandatory/parser/enhanced.c \
	mandatory/parser/stub_functions.c

SRC_LEXING = mandatory/parser/lexing/err.c \
	mandatory/parser/lexing/utils.c \
	mandatory/parser/lexing/extract.c \
	mandatory/parser/lexing/tokenize.c \
	mandatory/parser/lexing/variables.c \
	mandatory/parser/lexing/parse_var.c \
	mandatory/parser/lexing/expand.c

SRC_PARSING_CORE = mandatory/parser/parsing/ast/create.c \
	mandatory/parser/parsing/ast/free.c \
	mandatory/parser/parsing/logical/logical.c \
	mandatory/parser/parsing/logical/precedence.c \
	mandatory/parser/parsing/parentheses/check.c \
	mandatory/parser/parsing/parentheses/find.c

# PHASE 4-5: Ajout des modules de redirections
SRC_REDIRECTIONS = mandatory/parser/parsing/redir/redir.c \
	mandatory/parser/parsing/redir/input.c \
	mandatory/parser/parsing/redir/output.c \
	mandatory/parser/parsing/redir/validate.c \
	mandatory/parser/parsing/utils/redir.c

# Validation syntaxique
SRC_VALIDATION = mandatory/parser/parsing/validate/validate.c \
	mandatory/parser/parsing/validate/validation.c \
	mandatory/parser/parsing/validate/validation_utils.c \
	mandatory/parser/parsing/validate/validation_errors.c \
	mandatory/parser/parsing/validate/validation_advanced.c

# Combinaison finale AVEC redirections
SRC = $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_SIGNAL) $(SRC_EXECUTOR) \
	$(SRC_DEPLOYMENT) $(SRC_PARSER_BASE) $(SRC_LEXING) $(SRC_PARSING_CORE) $(SRC_REDIRECTIONS) $(SRC_VALIDATION)

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

DIR_OBJ	= obj
DIR		= $(DIR_OBJ)/
OBJ		= $(patsubst %.c, $(DIR)%.o, $(SRC))
