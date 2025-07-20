# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   pipes.mk                                           :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/07/19 16:00:00 by jjorda            #+#    #+#             #
#   Updated: 2025/07/19 16:00:00 by jjorda           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# *********************************** NAME *********************************** #

NAME_PP	= pipes_ut

# ********************************** PATHS *********************************** #

DIR_PP	= $(DIR_UT)
PSR		= $(MAN)/parser/parsing/
PIP		= $(PSR)/pipes/
UTI		= $(PSR)/utils/

# ********************************* SOURCES ********************************** #

SRC_PSR_PP = $(PSR)parser.c $(UTI)ast.c $(UTI)token.c
SRC_OPS_PP = $(PIP)operators.c $(PIP)pipes.c

SRC_PIPES = $(SRC_PSR_PP) $(SRC_OPS_PP) $(SRC_LXR) $(SRC_SUP) $(SRC_CUP)

UT_PIPES = $(PIP)main_test.c $(SRC_PIPES)

# ********************************* OBJECTS ********************************** #

OBJ_PP = $(patsubst %.c, $(DIR_PP)%.o, $(UT_PIPES))

# ********************************** RULES *********************************** #

pipes:	$(LIB) $(NAME_PP)
	@echo "$(GREEN)✓ Module pipes compilé avec succès$(NC)"
	@echo "$(YELLOW)Usage: ./$(NAME_PP)$(NC)"
	@echo "$(YELLOW)Script: ./test_pipes.sh$(NC)"

$(NAME_PP):	$(OBJ_PP)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PP) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_PP)

dir_pipes: 
	@mkdir -p $(DIR_PP)$(PIP)
	@mkdir -p $(DIR_PP)$(UTI)
	@mkdir -p $(DIR_PP)$(LEX)
	@mkdir -p $(DIR_PP)$(EXP)
	@mkdir -p $(DIR_PP)$(SUP)
	@mkdir -p $(DIR_PP)$(CUP)

$(DIR_PP)$(PSR)%.o: $(PSR)%.c | dir_pipes
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PP)$(PSR)utils/%.o: $(PSR)utils/%.c | dir_pipes
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PP)$(LEX)%.o: $(LEX)%.c | dir_pipes
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PP)$(EXP)%.o: $(EXP)%.c | dir_pipes
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PP)$(SUP)%.o: $(SUP)%.c | dir_pipes
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PP)$(CUP)%.o: $(CUP)%.c | dir_pipes
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_pipes:
	@rm -rf $(DIR_PP)$(PSR)
	@echo "$(RED)✗ Objets pipes supprimés$(NC)"

fclean_pipes: clean_pipes
	@rm -f $(NAME_PP)
	@echo "$(RED)✗ Exécutable pipes supprimé$(NC)"

re_pipes: fclean_pipes pipes

# ********************************* TESTING ********************************** #

test_pipes: pipes
	@echo "$(BLUE)=== TESTS AUTOMATISÉS PIPES ===$(NC)"
	@if [ -f "test_pipes.sh" ]; then \
		chmod +x test_pipes.sh && ./test_pipes.sh; \
	else \
		echo "$(RED)Erreur: test_pipes.sh non trouvé$(NC)"; \
	fi

test_manual_pipes: pipes
	@echo "$(BLUE)=== TESTS MANUELS PIPES ===$(NC)"
	@echo "$(YELLOW)Tests de base:$(NC)"
	@./$(NAME_PP)

# ********************************* BANNER *********************************** #

BAN_PP = \
" **********************************************" "\n" \
"*$(Y)   _____  _____ _____  _____ _____         $(O)*" "\n" \
"*$(Y)  |  _  ||     |  _  ||   __|   __|        $(O)*" "\n" \
"*$(Y)  |   __||  |  |   __||   __|__   |        $(O)*" "\n" \
"*$(Y)  |__|   |_____|__|   |_____|_____|        $(O)*" "\n" \
"*$(Y)                                           $(O)*" "\n" \
"*$(Y)   _____                     ___           $(O)*" "\n" \
"*$(Y)  |  _  |___ ___ ___ ___ ___ |  _|.1        $(O)*" "\n" \
"*$(Y)  |   __|   |  _|  _| -_|  _||  _|         $(O)*" "\n" \
"*$(Y)  |__|  |___|_| |___|___|_| |___|          $(O)*" "\n" \
"*$(V) Made by : alearroy / jjorda               $(O)*" "\n" \
"*$(V) Phase 6.1 : Support des pipes             $(O)*" "\n" \
"*$(V) Started : 19/07/2025                      $(O)*" "\n" \
"**********************************************"

# ********************************* COLORS *********************************** #

# Colors
GREEN	= \033[0;32m
RED		= \033[0;31m
YELLOW	= \033[1;33m
BLUE	= \033[0;34m
NC		= \033[0m

# ********************************* PHONY *********************************** #

.PHONY: pipes clean_pipes fclean_pipes re_pipes test_pipes test_manual_pipes
