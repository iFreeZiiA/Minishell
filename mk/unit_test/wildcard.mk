# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   wildcard.mk                                        :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/07/19 15:30:00 by jjorda            #+#    #+#             #
#   Updated: 2025/07/19 15:30:00 by jjorda           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# *********************************** NAME *********************************** #

NAME_WC	= wcard_ut

# ********************************** PATHS *********************************** #

DIR_WC	= $(DIR_UT)
WCD		= $(MAN)/parser/lexing/wildcard/

# ********************************* SOURCES ********************************** #

SRC_WCD	= $(WCD)wildcard.c $(WCD)match.c $(WCD)files.c $(WCD)tokens.c

UT_WCD	= $(WCD)main_test.c $(SRC_WCD) $(SRC_SUP) $(SRC_LXR) $(SRC_CUP)

# ********************************* OBJECTS ********************************** #

OBJ_WCD	= $(patsubst %.c, $(DIR_WC)%.o, $(UT_WCD))

# ********************************** RULES *********************************** #

wildcard:	$(LIB) $(NAME_WC)
	@echo "$(GREEN)✓ Module wildcard compilé avec succès$(NC)"
	@echo "$(YELLOW)Usage: ./$(NAME_WC) [\"commande avec wildcards\"]$(NC)"
	@echo "$(YELLOW)Script: ./test_wildcard.sh$(NC)"

$(NAME_WC):	$(OBJ_WCD)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_WCD) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_WC)

dir_wcard: 
	@mkdir -p $(DIR_WC)$(WCD)

$(DIR_WC)$(WCD)%.o: $(WCD)%.c | dir_wcard
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_WC)$(SUP)%.o: $(SUP)%.c | dir_wcard
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_WC)$(LEX)%.o: $(LEX)%.c | dir_wcard
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_WC)$(EXP)%.o: $(EXP)%.c | dir_wcard
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_WC)$(CUP)%.o: $(CUP)%.c | dir_wcard
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************** CLEAN *********************************** #

clean_wildcard:
	@rm -rf $(DIR_WC)$(WCD)
	@echo "$(RED)✗ Objets wildcard supprimés$(NC)"

fclean_wildcard: clean_wildcard
	@rm -f $(NAME_WC)
	@echo "$(RED)✗ Exécutable wildcard supprimé$(NC)"

re_wildcard: fclean_wildcard wildcard

# ********************************* TESTING ********************************** #

test_wildcard: wildcard
	@echo "$(BLUE)=== TESTS AUTOMATISÉS WILDCARD ===$(NC)"
	@if [ -f "test_wildcard.sh" ]; then \
		chmod +x test_wildcard.sh && ./test_wildcard.sh; \
	else \
		echo "$(RED)Erreur: test_wildcard.sh non trouvé$(NC)"; \
	fi

test_manual: wildcard
	@echo "$(BLUE)=== TESTS MANUELS WILDCARD ===$(NC)"
	@echo "$(YELLOW)Tests de base:$(NC)"
	@./$(NAME_WC)
	@echo "$(YELLOW)Test avec pattern *.c:$(NC)"
	@./$(NAME_WC) "*.c"
	@echo "$(YELLOW)Test avec pattern *test*:$(NC)"
	@./$(NAME_WC) "*test*"

# ********************************* BANNER *********************************** #

BAN_WC	= \
" **********************************************" "\n" \
"*$(Y)  _    _ _____ _      _____   _____  ___  ___  ___    $(O)*" "\n" \
"*$(Y) | |  | |_   _| |    |  __ \ / ____ / _ \|  _ \|  _ \   $(O)*" "\n" \
"*$(Y) | |  | | | | | |    | |  | | |    | |_| | |_) | | | |  $(O)*" "\n" \
"*$(Y) | |/\| | | | | |    | |  | | |    |  _  |  _ <| | | |  $(O)*" "\n" \
"*$(Y) \  /\  /_| |_| |____| |__| | |____| | | | |_) | |_| |  $(O)*" "\n" \
"*$(Y)  \/  \/ \___/\_____/|_____/ \_____|_| |_|____/|____/   $(O)*" "\n" \
"*$(V) Made by : alearroy / jjorda                           $(O)*" "\n" \
"*$(V) Started : 19/07/2025                                  $(O)*" "\n" \
"*$(V) Finished : 19/07/2025                                 $(O)*" "\n" \
"**********************************************"

# ********************************* PHONY *********************************** #

.PHONY: wildcard clean_wildcard fclean_wildcard re_wildcard test_wildcard test_manual
