# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    logical.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/20 00:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/20 17:11:25 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ********************************* LOGICAL ********************************** #

NAME_LG	= logical_ut

# ********************************** PATHS *********************************** #

DIR_LG	= $(DIR_UT)
PRS		= $(MAN)/parser/parsing/
LOG_DIR	= $(PRS)logical/
UTI		= $(PRS)utils/

# Sources du module logical parsing
SRC_LOG	= $(LOG_DIR)logical.c $(LOG_DIR)precedence.c $(UTI)logical.c

# Unit test logical parsing
UT_LG	= $(LOG_DIR)main_test.c $(SRC_LOG) $(SRC_LXR) $(SRC_SUP) $(SRC_CUP) \
		$(SRC_PSR)

OBJ_LG	= $(patsubst %.c, $(DIR_LG)%.o, $(UT_LG))

# ********************************** RULES *********************************** #

logical: $(LIB) $(NAME_LG)

$(NAME_LG): $(OBJ_LG)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_LG) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_LG)

dir_logical:
	@mkdir -p $(DIR_LG)$(LOG_DIR)
	@mkdir -p $(DIR_LG)$(UTI)
	@mkdir -p $(DIR_LG)$(LEX)
	@mkdir -p $(DIR_LG)$(EXP)
	@mkdir -p $(DIR_LG)$(WCD)
	@mkdir -p $(DIR_LG)$(SUP)
	@mkdir -p $(DIR_LG)$(CUP)
	@mkdir -p $(DIR_LG)$(PRR)utils

$(DIR_LG)$(LOG_DIR)%.o: $(LOG_DIR)%.c | dir_logical
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_LG)$(LEX)%.o: $(LEX)%.c | dir_logical
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_LG)$(EXP)%.o: $(EXP)%.c | dir_logical
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_LG)$(WCD)%.o: $(WCD)%.c | dir_logical
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_LG)$(SUP)%.o: $(SUP)%.c | dir_logical
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_LG)$(CUP)%.o: $(CUP)%.c | dir_logical
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_LG)$(PRR)%.o: $(PRR)%.c | dir_logical
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_logical:
	@$(RM) -f $(OBJ_LG)

fclean_logical: clean_logical
	@$(RM) -f $(NAME_LG)

re_logical: fclean_logical logical

# ********************************* TESTS *********************************** #

test_logical: logical
	@echo "$(BLUE)=== TESTS LOGICAL OPERATORS ===$(NC)"
	@echo "$(YELLOW)Test de base:$(NC)"
	@./$(NAME_LG)
	@echo "$(YELLOW)Test avec opérateurs logiques:$(NC)"
	@echo "echo hello && echo world" | ./$(NAME_LG)
	@echo "$(YELLOW)Test avec priorités:$(NC)"
	@echo "true || false && echo test" | ./$(NAME_LG)

# ********************************* BANNER *********************************** #

BAN_LG = \
" **********************************************" "\n" \
"*$(Y)   _       _______ _______ _____ ______  _     $(O)*" "\n" \
"*$(Y)  | |     (_______|_______|_____|_____ \| |    $(O)*" "\n" \
"*$(Y)  | |      _     _ _____      _   _____) ) |    $(O)*" "\n" \
"*$(Y)  | |     | |   | |  ___)    | | (_____ (| |    $(O)*" "\n" \
"*$(Y)  | |_____| |___| | |_____  _| |_ _____) |_|    $(O)*" "\n" \
"*$(Y)  |_______)_______|_______)_____|______/(_)    $(O)*" "\n" \
"*$(V) LOGICAL PARSING - Made by jjorda              $(O)*" "\n" \
"*$(V) Phase 7.1 - Opérateurs logiques && ||        $(O)*" "\n" \
"*$(V) Started : 20/07/2025                          $(O)*" "\n" \
"**********************************************"

# ********************************* PHONY *********************************** #

.PHONY: logical clean_logical fclean_logical re_logical test_logical
