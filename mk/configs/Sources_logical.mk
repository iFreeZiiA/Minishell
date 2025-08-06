# Sources pour opérateurs logiques seulement (sans parenthèses)

# Définitions des répertoires
MAN		 = mandatory/
PSR		= $(MAN)parser/
LEX		= $(PSR)lexing/
PSI		= $(PSR)parsing/
BLT		= $(MAN)builtin/
SGL		= $(MAN)signal/
SUP		= $(MAN)deployment/setup/
CUP		= $(MAN)deployment/cleanup/
EXE		= $(MAN)executor/

# Sources principales
SRC_MAIN = $(MAN)main.c

SRC_BUILTIN = $(MAN)builtin/builtin_cd_utils.c $(MAN)builtin/builtin_pwd.c \
	$(MAN)builtin/run_builtin.c $(MAN)builtin/builtin_unset.c \
	$(MAN)builtin/builtin_export.c $(MAN)builtin/builtin_env.c \
	$(MAN)builtin/builtin_exit.c $(MAN)builtin/builtin_echo.c \
	$(MAN)builtin/builtin_cd.c

SRC_SIGNAL = $(MAN)signal/signal.c

SRC_EXECUTOR = $(MAN)executor/executor.c $(MAN)executor/ast_to_command_list.c \
	$(MAN)executor/get_path.c

SRC_DEPLOYMENT = $(MAN)deployment/setup/setup.c $(MAN)deployment/setup/environment.c \
	$(MAN)deployment/cleanup/cleanup.c

# Parser
SRC_PARSER_BASE = $(MAN)parser/enhanced.c $(MAN)parser/parser.c $(MAN)parser/stub_functions.c

SRC_LEXING = $(MAN)parser/lexing/err.c $(MAN)parser/lexing/utils.c \
	$(MAN)parser/lexing/extract.c $(MAN)parser/lexing/tokenize.c \
	$(MAN)parser/lexing/variables.c $(MAN)parser/lexing/parse_var.c \
	$(MAN)parser/lexing/expand.c

# Parsing - SANS parentheses
SRC_PARSING_LOGICAL = $(MAN)parser/parsing/ast/create.c \
	$(MAN)parser/parsing/ast/free.c \
	$(MAN)parser/parsing/logical/logical.c \
	$(MAN)parser/parsing/logical/precedence.c \
	$(MAN)parser/parsing/pipes/operators.c \
	$(MAN)parser/parsing/pipes/pipes.c \
	$(MAN)parser/parsing/redir/input.c \
	$(MAN)parser/parsing/redir/output.c \
	$(MAN)parser/parsing/redir/redir.c \
	$(MAN)parser/parsing/redir/validate.c \
	$(MAN)parser/parsing/utils/redir.c \
	$(MAN)parser/parsing/parse.c

SRC_PARSING_VALIDATE = $(MAN)parser/parsing/validate/validate.c \
	$(MAN)parser/parsing/validate/validation.c \
	$(MAN)parser/parsing/validate/validation_utils.c \
	$(MAN)parser/parsing/validate/validation_errors.c \
	$(MAN)parser/parsing/validate/validation_advanced.c

# Sources complètes pour opérateurs logiques
SRC = $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_SIGNAL) $(SRC_EXECUTOR) \
	$(SRC_DEPLOYMENT) $(SRC_PARSER_BASE) $(SRC_LEXING) $(SRC_PARSING_LOGICAL) $(SRC_PARSING_VALIDATE)

# Libs
LIB			= libs
DIR_LIB		= ./lib
LIBS		= -L$(DIR_LIBFT) -lft -L$(DIR_LIBMS) -lms -lreadline

DIR_LIBFT	= $(DIR_LIB)/libft
LIBFT		= $(DIR_LIBFT)/libft.a

DIR_LIBMS	= $(DIR_LIB)/libms
LIBMS		= $(DIR_LIBMS)/libms.a

# Objects
DIR			= obj/
DIR_OBJ		= obj

OBJ			= $(patsubst %.c, $(DIR)%.o, $(SRC))
