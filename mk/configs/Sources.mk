# *********************************** FILES *********************************** #

#  								    MANDATORY								  #
MAN		= mandatory/
SRC		= $(MAN)main.c

#  								      BONUS  								  #
# BNS		= bonus/
# BNSC		= _bonus.c
# SRC_BNS	= $(BNS)main.c

# ************************************ LIB *********************************** #

LIB		= libft.a
DIR_LIB	= ./libft

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