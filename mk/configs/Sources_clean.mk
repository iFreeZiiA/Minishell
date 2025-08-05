# *********************************** FILES *********************************** #

#  								    MANDATORY								  #
# Définitions des répertoires
MAN		 = mandatory/
PSR		= $(MAN)parser/
LEX		= $(PSR)lexing/
EXP		= $(LEX)expansion/
WCD		= $(LEX)wildcard/
PSI		= $(PSR)parsing/
BLT		= $(MAN)builtin/
SGL		= $(MAN)signal/
SUP		= $(MAN)deployment/setup/
CUP		= $(MAN)deployment/cleanup/
EXE		= $(MAN)executor/

# ******************************* SOURCES CLEAN ****************************** #

# Liste explicite de tous les fichiers .c dans mandatory/ (excluant les tests)
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

# New clean parser structure
SRC_LEXING = $(MAN)parser/src/lexing/lexer.c

SRC_PARSING_UTILS = $(MAN)parser/src/parsing/utils/ast.c \
	$(MAN)parser/src/parsing/utils/logical.c $(MAN)parser/src/parsing/utils/parser.c \
	$(MAN)parser/src/parsing/utils/redir.c $(MAN)parser/src/parsing/utils/token.c

SRC_PARSING_FEATURES = $(MAN)parser/src/parsing/groups/finder.c \
	$(MAN)parser/src/parsing/groups/parser.c $(MAN)parser/src/parsing/groups/validator.c \
	$(MAN)parser/src/parsing/pipe/detector.c $(MAN)parser/src/parsing/pipe/parser.c \
	$(MAN)parser/src/parsing/logic/parser.c $(MAN)parser/src/parsing/logic/precedence.c \
	$(MAN)parser/src/parsing/redirect/input.c $(MAN)parser/src/parsing/redirect/output.c \
	$(MAN)parser/src/parsing/redirect/parser.c $(MAN)parser/src/parsing/redirect/validator.c \
	$(MAN)parser/src/parsing/heredoc/heredoc.c

SRC_UTILS = $(MAN)utils/missing_functions.c

# Version unifiée des sources
SRC		= $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_SIGNAL) $(SRC_EXECUTOR) \
	$(SRC_DEPLOYMENT) $(SRC_LEXING) \
	$(SRC_PARSING_UTILS) $(SRC_PARSING_FEATURES) $(SRC_UTILS)

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
