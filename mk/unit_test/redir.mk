# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    redirections.mk                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/19 16:30:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/19 16:30:00 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ********************************* REDIRECTIONS ***************************** #

NAME_RDR = redirections_ut

DIR_RDR	= $(DIR_UT)
PRS		= $(MAN)/parser/parsing/
RDR		= $(PRS)/redir/
UTI		= $(PRS)/utils/
PIP		= $(PRS)/pipes/

# Sources du module redirections Phase 6.2
SRC_RDR	= $(RDR)redir.c $(RDR)input.c $(RDR)output.c $(RDR)validate.c \
	$(UTI)redir.c $(UTI)parser.c $(PRS)parser.c $(PIP)operators.c		\
	$(PIP)pipes.c

# Unit test redirections Phase 6.2
UT_RDR	= $(RDR)main_test.c $(SRC_RDR) $(SRC_SUP) $(SRC_LXR)

OBJ_RDR	= $(patsubst %.c, $(DIR_RDR)%.o, $(UT_RDR))

# ********************************** RULES *********************************** #

redir: $(LIB) $(NAME_RDR)

$(NAME_RDR): $(OBJ_RDR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_RDR) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_RDR)

dir_redirections:
	@mkdir -p $(DIR_RDR)$(RDR)
	@mkdir -p $(DIR_RDR)$(UTI)
	@mkdir -p $(DIR_RDR)$(PIP)
	@mkdir -p $(DIR_RDR)$(SUP)
	@mkdir -p $(DIR_RDR)$(LEX)
	@mkdir -p $(DIR_RDR)$(EXP)

$(DIR_RDR)$(RDR)%.o: $(RDR)%.c | dir_redirections
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_RDR)$(SUP)%.o: $(SUP)%.c | dir_redirections
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_RDR)$(LEX)%.o: $(LEX)%.c | dir_redirections
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_RDR)$(EXP)%.o: $(EXP)%.c | dir_redirections
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_redirections:
	@$(RM) -f $(OBJ_RDR)

fclean_redirections: clean_redirections
	@$(RM) -f $(NAME_RDR)

re_redirections: fclean_redirections redirections

# ********************************* TESTS *********************************** #

test_redirections: redirections
	@echo "$(BLUE)=== TESTS REDIRECTIONS PHASE 6.2 ===$(NC)"
	@echo "$(YELLOW)Test de base:$(NC)"
	@./$(NAME_RDR)
	@echo "$(YELLOW)Tests complets avec script:$(NC)"
	@if [ -f "test_redirections.sh" ]; then \
		chmod +x test_redirections.sh && ./test_redirections.sh; \
	else \
		echo "$(RED)Erreur: test_redirections.sh non trouvé$(NC)"; \
	fi

test_manual_redirections: redirections
	@echo "$(BLUE)=== TESTS MANUELS REDIRECTIONS ===$(NC)"
	@echo "$(YELLOW)Test création redirection:$(NC)"
	@./$(NAME_RDR)
	@echo "$(YELLOW)Test avec fichiers:$(NC)"
	@echo "test" > test_input.txt
	@echo "echo hello > output.txt" | ./lexer_ut
	@echo "cat < test_input.txt" | ./lexer_ut
	@rm -f test_input.txt output.txt

# ********************************* BANNER *********************************** #

BAN_RDR = \
" **********************************************" "\n" \
"*$(Y)  _______ _______ ______  _____ ______     $(O)*" "\n" \
"*$(Y) (______ (_______|  ___ \(____ (_____ \    $(O)*" "\n" \
"*$(Y)  _____) )_____  | |   | _   | )_____) )   $(O)*" "\n" \
"*$(Y) |  __  /|  ___) | |   | | | | |  __  /    $(O)*" "\n" \
"*$(Y) | |  \ \| |_____| |__/ /_ | | | |  \ \     $(O)*" "\n" \
"*$(Y) |_|   |_|_______)_____/(__| |_|_|   |_|    $(O)*" "\n" \
"*$(V) REDIRECTIONS PHASE 6.2 - Made by jjorda   $(O)*" "\n" \
"*$(V) Parsing < > >> << pour commandes          $(O)*" "\n" \
"*$(V) Started : 19/07/2025                       $(O)*" "\n" \
"**********************************************"

# ********************************* PHONY *********************************** #

.PHONY: redirections clean_redirections fclean_redirections re_redirections test_redirections test_manual_redirections
