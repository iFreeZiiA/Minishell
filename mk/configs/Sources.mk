# *********************************** FILES *********************************** #

#  								    MANDATORY								  #
MAN		= mandatory/
LEX		= $(MAN)/parser/lexing/
PRR		= $(MAN)parser/parsing/
SUP		= $(MAN)/deployment/setup/
CUP		= $(MAN)/deployment/cleanup/
EXP		= $(LEX)expansion/
BLT		= $(MAN)/builtin/
SGL		= $(MAN)/signal/

# Define source files for each module
SRC_LXR = $(LEX)lexing.c $(LEX)err.c $(LEX)utils.c $(EXP)expansion.c		\
	$(EXP)expansion_utils.c $(EXP)exit_code.c $(EXP)exp_dquote.c			\
	$(EXP)exp_tok.c

SRC_PRR	= $(PRR)parsing.c $(PRR)expression.c $(PRR)command.c			\
	$(PRR)redirections.c $(PRR)ast_utils.c

SRC_SUP	= $(SUP)setup.c $(SUP)environment.c

SRC_CUP	= $(CUP)cleanup.c

SRC_BLT = $(BLT)/builtin_echo.c $(BLT)/builtin_pwd.c $(BLT)/run_builtin.c	\
	$(BLT)/builtin_cd.c $(BLT)/builtin_env.c $(BLT)/builtin_exit.c 			\
	$(BLT)/builtin_export.c $(BLT)/builtin_unset.c $(BLT)/builtin_cd_utils.c

SRC_SGL = $(SGL)/signal.c

SRC		= $(MAN)/main.c $(SRC_LXR) $(SRC_PRR) $(SRC_SUP) $(SRC_CUP)			\
	$(SRC_BLT) $(SRC_SGL)

DIRS	= dir_cleanup dir_parser dir_subshell

#  								      BONUS  								  #
# BNS		= bonus/
# BNSC		= _bonus.c
# SRC_BNS	= $(BNS)main.c

#  								    UNIT_TEST 								  #

DIR_UT		= $(DIR_OBJ)/unit_test/
UT			= $(LEX) $(PRR) $(SUP) $(CUP)

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
