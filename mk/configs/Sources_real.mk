# *********************************** FILES *********************************** #

#  								    MANDATORY								  #
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

# ******************************* SOURCES REAL FILES ******************************* #

# Liste basée sur les fichiers qui existent réellement
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

# Parser files that actually exist
SRC_PARSER_BASE = $(MAN)parser/enhanced.c $(MAN)parser/parser.c

SRC_LEXING = $(MAN)parser/lexing/err.c $(MAN)parser/lexing/utils.c \
	$(MAN)parser/lexing/extract.c $(MAN)parser/lexing/tokenize.c \
	$(MAN)parser/lexing/variables.c $(MAN)parser/lexing/parse_var.c \
	$(MAN)parser/lexing/expand.c

SRC_PARSING_AST = $(MAN)parser/parsing/ast/create.c \
	$(MAN)parser/parsing/ast/free.c $(MAN)parser/parsing/ast/print.c

SRC_PARSING_EXPRESSION = $(MAN)parser/parsing/expression/command.c \
	$(MAN)parser/parsing/expression/group.c $(MAN)parser/parsing/expression/redirections.c

SRC_PARSING_LOGICAL = $(MAN)parser/parsing/logical/logical.c \
	$(MAN)parser/parsing/logical/precedence.c

SRC_PARSING_PARENTHESES = $(MAN)parser/parsing/parentheses/check.c \
	$(MAN)parser/parsing/parentheses/find.c $(MAN)parser/parsing/parentheses/parentheses.c

SRC_PARSING_PIPES = $(MAN)parser/parsing/pipes/operators.c \
	$(MAN)parser/parsing/pipes/pipes.c

SRC_PARSING_REDIR = $(MAN)parser/parsing/redir/input.c \
	$(MAN)parser/parsing/redir/output.c $(MAN)parser/parsing/redir/redir.c \
	$(MAN)parser/parsing/redir/validate.c

SRC_PARSING_UTILS = $(MAN)parser/parsing/utils/ast.c \
	$(MAN)parser/parsing/utils/logical.c $(MAN)parser/parsing/utils/parser.c \
	$(MAN)parser/parsing/utils/redir.c $(MAN)parser/parsing/utils/token.c

SRC_PARSING_VALIDATE = $(MAN)parser/parsing/validate/ast.c \
	$(MAN)parser/parsing/validate/ast_optimizer.c $(MAN)parser/parsing/validate/precedence.c \
	$(MAN)parser/parsing/validate/validate.c \
	$(MAN)parser/parsing/validate/validation.c \
	$(MAN)parser/parsing/validate/validation_utils.c \
	$(MAN)parser/parsing/validate/validation_errors.c \
	$(MAN)parser/parsing/validate/validation_advanced.c

SRC_PARSING_OTHER = $(MAN)parser/parsing/parse.c $(MAN)parser/parsing/parser.c \
	$(MAN)parser/parsing/parser_util.c $(MAN)parser/parsing/utils.c \
	$(MAN)parser/parsing/heredoc/heredoc.c $(MAN)parser/parsing/logic_operator/parsing_utils.c \
	$(MAN)parser/parsing/logic_operator/print_utils.c

SRC_UTILS = $(MAN)utils/missing_functions.c

# Version unifiée des sources
SRC		= $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_SIGNAL) $(SRC_EXECUTOR) \
	$(SRC_DEPLOYMENT) $(SRC_PARSER_BASE) $(SRC_LEXING) $(SRC_PARSING_AST) \
	$(SRC_PARSING_EXPRESSION) $(SRC_PARSING_LOGICAL) $(SRC_PARSING_PARENTHESES) \
	$(SRC_PARSING_PIPES) $(SRC_PARSING_REDIR) $(SRC_PARSING_UTILS) \
	$(SRC_PARSING_VALIDATE) $(SRC_PARSING_OTHER) $(SRC_UTILS)

#  								      BONUS  								  #
# BNS		= bonus/
# BNSC		= _bonus.c
# SRC_BNS	= $(BNS)main.c

#  								    UNIT_TEST 								  #

DIR_UT		= $(DIR_OBJ)/unit_test/
UT			= $(LEX) $(PSR) $(SUP) $(CUP)

NAME_UT		= $(NAME_LX) $(NAME_PR) $(NAME_SUP) $(NAME_CUP)
NAME_ALL	= $(NAME_A) $(NAME_J)

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
DIR_BNS	= $(DIR_OBJ)/
OBJ		= $(patsubst %.c, $(DIR)%.o, $(SRC))
OBJ_BNS	= $(patsubst %.c, $(DIR_BNS)%.o, $(SRC_BNS))

BANNER	= \
" **********************************************" "\n" \
"*$(Y) ______    _     _ _______ _       _        $(O)*" "\n" \
"*$(Y)|  ___ \  | |   | (_______) |     | |       $(O)*" "\n" \
"*$(Y)| | _ | | | |__ | |_____  | |     | |       $(O)*" "\n" \
"*$(Y)| || || | |  __)| |  ___) | |     | |       $(O)*" "\n" \
"*$(Y)| || || |_| |   | | |_____| |_____| |_____  $(O)*" "\n" \
"*$(Y)|_||_||_(_)_|   |_|_______)_______)_______) $(O)*" "\n" \
"*$(V) Made by : alearroy / jjorda                $(O)*" "\n" \
"*$(V) Started : 04/04/2025                       $(O)*" "\n" \
"*$(V) Finished :                                 $(O)*" "\n" \
"**********************************************"
