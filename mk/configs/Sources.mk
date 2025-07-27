# *********************************** FILES *********************************** #

#  								    MANDATORY								  #
# dir_parsing supprimé pour éviter les conflits avec les unit tests

# ******************************** COMPILATION ******************************* #

$(DIR_PSI)$(PSR)%.o: $(PSR)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_U)%.o: $(PSR_U)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

# $(DIR_PSI)$(PSR_OPS)%.o: $(PSR_OPS)%.c | dir_parsing
# 	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_PIP)%.o: $(PSR_PIP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_LOG)%.o: $(PSR_LOG)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_PAR)%.o: $(PSR_PAR)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_RED)%.o: $(PSR_RED)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_HER)%.o: $(PSR_HER)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_WLD)%.o: $(PSR_WLD)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_VAL)%.o: $(PSR_VAL)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(LEX)%.o: $(LEX)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(EXP)%.o: $(EXP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(SUP)%.o: $(SUP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(CUP)%.o: $(CUP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

MAN		 = mandatory/
PSR		= $(MAN)parser/
LEX		= $(PSR)lexing/
EXP		= $(LEX)expansion/
EXP		= $(LEX)wildcard/
PSI		= $(PSR)parsing/
HER 	= $(PSR)heredoc/
LOG 	= $(PSR)logical/
PAR 	= $(PSR)parentheses/
PIP 	= $(PSR)pipes/
RED 	= $(PSR)redir/
UTI		= $(PSR)utils/
VAL 	= $(PSR)validate/
BLT		= $(MAN)builtin/
SGL		= $(MAN)signal/

SRC_LXR = $(LEX)lexing.c $(LEX)err.c $(LEX)utils.c $(EXP)expansion.c		\
	$(EXP)utils.c $(EXP)exit_code.c $(EXP)dquote.c $(EXP)tok.c				\
	$(WCD)wildcard.c $(WCD)files.c $(WCD)match.c $(WCD)tokens.c

SRC_HER	= $(HER)heredoc.c

SRC_LOG	= $(LOG)logical.c $(LOG)precedence.c

SRC_PRT = $(PRT)parentheses.c

SRC_PIP	= $(PIP)operators.c $(PIP)pipes.c

SRC_RED	= $(RED)input.c $(RED)output.c $(RED)redir.c $(RED)validate.c

SRC_UTI	= $(UTI)ast.c $(UTI)logical.c $(UTI)parser.c $(UTI)redir.c $(UTI)token.c

SRC_VAL	= $(VAL)ast_optimizer.c $(VAL)precedence.c $(VAL)validate.c

SRC_PSR	= $(PSR)parse.c $(PSR)parser_util.c $(PSR)parser.c

SRC_PSI	= $(PSI)enhanced.c $(PSI)parser.c $(SRC_HER) $(SRC_PRT) $(SRC_PIP)	\
	$(SRC_RED) $(SRC_UTI) $(SRC_VAL) $(SRC_PSR)

SRC_SUP	= $(SUP)setup.c $(SUP)environment.c

SRC_CUP	= $(CUP)cleanup.c

SRC_BLT = $(BLT)/builtin_echo.c $(BLT)/builtin_pwd.c $(BLT)/run_builtin.c	\
	$(BLT)/builtin_cd.c $(BLT)/builtin_env.c $(BLT)/builtin_exit.c 			\
	$(BLT)/builtin_export.c $(BLT)/builtin_unset.c $(BLT)/builtin_cd_utils.c

SRC_SGL = $(SGL)/signal.c

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

SRC_PARSER_BASE = $(MAN)parser/enhanced.c $(MAN)parser/parser.c

SRC_LEXING = $(MAN)parser/lexing/lexing.c $(MAN)parser/lexing/err.c \
	$(MAN)parser/lexing/utils.c

SRC_EXPANSION = $(MAN)parser/lexing/expansion/tok.c \
	$(MAN)parser/lexing/expansion/dquote.c \
	$(MAN)parser/lexing/expansion/utils.c \
	$(MAN)parser/lexing/expansion/expansion.c \
	$(MAN)parser/lexing/expansion/exit_code.c

SRC_WILDCARD = $(MAN)parser/lexing/wildcard/wildcard.c \
	$(MAN)parser/lexing/wildcard/match.c \
	$(MAN)parser/lexing/wildcard/files.c \
	$(MAN)parser/lexing/wildcard/tokens.c

SRC_PARSING_UTILS = $(MAN)parser/parsing/utils/ast.c \
	$(MAN)parser/parsing/utils/logical.c $(MAN)parser/parsing/utils/parser.c \
	$(MAN)parser/parsing/utils/redir.c $(MAN)parser/parsing/utils/token.c

SRC_PARSING_FEATURES = $(MAN)parser/parsing/parentheses/parentheses.c \
	$(MAN)parser/parsing/pipes/operators.c $(MAN)parser/parsing/pipes/pipes.c \
	$(MAN)parser/parsing/logical/logical.c $(MAN)parser/parsing/logical/precedence.c \
	$(MAN)parser/parsing/redir/input.c $(MAN)parser/parsing/redir/output.c \
	$(MAN)parser/parsing/redir/redir.c $(MAN)parser/parsing/redir/validate.c \
	$(MAN)parser/parsing/heredoc/heredoc.c \
	$(MAN)parser/parsing/validate/ast_optimizer.c $(MAN)parser/parsing/validate/ast.c \
	$(MAN)parser/parsing/validate/precedence.c $(MAN)parser/parsing/validate/validate.c

SRC_UTILS = $(MAN)utils/missing_functions.c

SRC		= $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_SIGNAL) $(SRC_EXECUTOR) \
	$(SRC_DEPLOYMENT) $(SRC_PARSER_BASE) $(SRC_LEXING) $(SRC_EXPANSION) \
	$(SRC_WILDCARD) $(SRC_PARSING_UTILS) $(SRC_PARSING_FEATURES) $(SRC_UTILS)

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
