# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    heredoc.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/26 16:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/26 09:30:56 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ********************************* HEREDOC ********************************** #

NAME_HER = heredoc_ut

# ********************************** PATHS *********************************** #

DIR_HER	= $(DIR_UT)
HER_DIR	= $(PSR)heredoc/

# Sources du module heredoc parsing Phase 10
SRC_HER	= $(HER_DIR)heredoc.c

# Unit test heredoc parsing
UT_HER	= $(HER_DIR)main_test.c $(SRC_HER) $(SRC_PSR) $(SRC_LXR) $(SRC_SUP)

OBJ_HER	= $(patsubst %.c, $(DIR_HER)%.o, $(UT_HER))

# ********************************** RULES *********************************** #

heredoc: $(LIB) $(NAME_HER)
	@echo "$(GREEN)✓ Module heredoc compilé avec succès$(NC)"
	@echo "$(YELLOW)Usage: ./$(NAME_HER)$(NC)"
	@echo "$(YELLOW)Script: ./test_heredoc.sh$(NC)"

$(NAME_HER): $(OBJ_HER)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_HER) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_HER)

dir_heredoc: dir_setup
	@mkdir -p $(DIR_HER)$(HER_DIR)
	@mkdir -p $(DIR_HER)$(PSR_U)
	@mkdir -p $(DIR_HER)$(LEX)
	@mkdir -p $(DIR_HER)$(EXP)
	@mkdir -p $(DIR_HER)$(SUP)

$(DIR_HER)$(HER_DIR)%.o: $(HER_DIR)%.c | dir_heredoc
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_HER)$(PSR)%.o: $(PSR)%.c | dir_heredoc
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_HER)$(PSR_U)%.o: $(PSR_U)%.c | dir_heredoc
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_HER)$(LEX)%.o: $(LEX)%.c | dir_heredoc
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_HER)$(EXP)%.o: $(EXP)%.c | dir_heredoc
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_HER)$(SUP)%.o: $(SUP)%.c | dir_heredoc
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_heredoc:
	@rm -rf $(DIR_HER)$(HER_DIR)
	@echo "$(RED)✗ Objets heredoc supprimés$(NC)"

fclean_heredoc: clean_heredoc
	@rm -f $(NAME_HER)
	@echo "$(RED)✗ Exécutable heredoc supprimé$(NC)"

re_heredoc: fclean_heredoc heredoc

# ********************************* TESTING ********************************** #

test_heredoc_auto: heredoc
	@echo "$(BLUE)=== TESTS AUTOMATISÉS HEREDOC PHASE 10 ===$(NC)"
	@if [ -f "test_heredoc.sh" ]; then \
		chmod +x test_heredoc.sh; \
		./test_heredoc.sh; \
	else \
		echo "$(RED)❌ Script test_heredoc.sh non trouvé$(NC)"; \
		echo "$(YELLOW)💡 Créez le script test_heredoc.sh pour les tests shell$(NC)"; \
	fi

test_heredoc_c: $(NAME_HER)
	@echo "$(BLUE)=== TESTS C HEREDOC PHASE 10 ===$(NC)"
	@./$(NAME_HER)

# Tests complets (C + Shell)
test_heredoc: test_heredoc_c test_heredoc_auto
	@echo "$(GREEN)✅ Tests heredoc complets terminés !$(NC)"

# ********************************* BANNER *********************************** #

BAN_HER = \
"**********************************************" "\n" \
"*$(Y)      _  _ _______ _______ _______ ____   $(O)*" "\n" \
"*$(Y)     | || (_______|_______|_______)  _ \  $(O)*" "\n" \
"*$(Y)     | || |_____   _____   _      | | | | $(O)*" "\n" \
"*$(Y)     | ||_|  ___)  |  ___)| |     | | | | $(O)*" "\n" \
"*$(Y)     | | | |_____ | |_____| |_____| |/ /  $(O)*" "\n" \
"*$(Y)     |_| |_______)_______)_______)___/   $(O)*" "\n" \
"*$(V) Made by : alearroy / jjorda                $(O)*" "\n" \
"*$(V) Started : 04/04/2025                       $(O)*" "\n" \
"*$(V) Finished :                                 $(O)*" "\n" \
"**********************************************"

# ********************************* PHONY *********************************** #

.PHONY: heredoc clean_heredoc fclean_heredoc re_heredoc test_heredoc \
        test_heredoc_c test_heredoc_auto dir_heredoc
		