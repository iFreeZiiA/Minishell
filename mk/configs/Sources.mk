# *********************************** FILES *********************************** #

#  								    MANDATORY								  #
MAN		= mandatory/
SRC		= $(MAN)main.c $(SRC_LXR) $(SRC_PRR) $(SRC_SUP) $(SRC_CUP)

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

LIB			= lib
DIR_LIB		= ./lib
LIBS		= -L./$(DIR_LIBFT) -lft -L./$(DIR_LIBMS) -lms

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
"**********************************************" "\n" \
"*$(Y)   ______       _    _  _  _        ______  $(O)*" "\n" \
"*$(Y)  (_____ \     | |  | || || |  /\  (_____ \ $(O)*" "\n" \
"*$(Y)   _____) )     \ \ | || || | /  \  _____) )$(O)*" "\n" \
"*$(Y)  |  ____/       \ \| ||_|| |/ /\ \|  ____/ $(O)*" "\n" \
"*$(Y)  | |_______ _____) ) |___| | |__| | |      $(O)*" "\n" \
"*$(Y)  |_(_______|______/ \______|______|_|      $(O)*" "\n" \
"*$(V) Made by : alearroy / jjorda                $(O)*" "\n" \
"*$(V) Started : 04/04/2025                       $(O)*" "\n" \
"*$(V) Finished :                                 $(O)*" "\n" \
"**********************************************"