# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    parentheses.mk                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/26 00:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/26 08:59:27 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ******************************** PARENTHESES ******************************* #

NAME_PRT = test_parentheses
NAME_PRT_UTIL = parentheses_ut

DIR_PRT	= $(DIR_UT)
PRT		= $(MAN)/parser/parsing/parentheses/

# Sources du module parentheses Phase 8.1
SRC_PRT	= $(PRT)parentheses.c

# Unit test parentheses Phase 8.1 - Main test
UT_PRT	= $(PRT)main_test.c $(SRC_PRT) $(SRC_LXR) $(SRC_SUP) $(SRC_PSR)

# Objets compilation
OBJ_PRT = $(patsubst %.c, $(DIR_PRT)%.o, $(UT_PRT))

# ********************************** RULES *********************************** #

parentheses: $(LIB) $(NAME_PRT)
	@echo "$(GREEN)✓ Module parentheses Phase 8.1 compilé avec succès$(NC)"
	@echo "$(YELLOW)Usage: ./$(NAME_PRT)$(NC)"
	@echo "$(YELLOW)Script: ./test_paren.sh$(NC)"

$(NAME_PRT): $(OBJ_PRT)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PRT) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_PRT)

dir_parentheses: dir_setup
	@mkdir -p $(DIR_PRT)$(PRT)
	@mkdir -p $(DIR_PRT)$(PSR)
	@mkdir -p $(DIR_PRT)$(PSR_U)
	@mkdir -p $(DIR_PRT)$(LEX)
	@mkdir -p $(DIR_PRT)$(EXP)
	@mkdir -p $(DIR_PRT)$(SUP)

$(DIR_PRT)$(PRT)%.o: $(PRT)%.c | dir_parentheses
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PRT)$(PSR)%.o: $(PSR)%.c | dir_parentheses
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PRT)$(PSR_U)%.o: $(PSR_U)%.c | dir_parentheses
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PRT)$(LEX)%.o: $(LEX)%.c | dir_parentheses
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PRT)$(EXP)%.o: $(EXP)%.c | dir_parentheses
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PRT)$(SUP)%.o: $(SUP)%.c | dir_parentheses
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_parentheses:
	@rm -rf $(DIR_PRT)$(PRT)
	@echo "$(RED)✗ Objets parentheses supprimés$(NC)"

fclean_parentheses: clean_parentheses
	@rm -f $(NAME_PRT)
	@echo "$(RED)✗ Exécutable parentheses supprimé$(NC)"

re_parentheses: fclean_parentheses parentheses

# ********************************* TESTING ********************************** #

test_parentheses_auto: parentheses
	@echo "$(BLUE)=== TESTS AUTOMATISÉS PARENTHÈSES PHASE 8.1 ===$(NC)"
	@if [ -f "test_paren.sh" ]; then \
		chmod +x test_paren.sh; \
		./test_paren.sh; \
	else \
		echo "$(RED)❌ Script test_paren.sh non trouvé$(NC)"; \
		echo "$(YELLOW)💡 Créez le script test_paren.sh pour les tests shell$(NC)"; \
	fi

test_parentheses_c: $(NAME_PRT)
	@echo "$(BLUE)=== TESTS C - PARENTHÈSES PHASE 8.1 ===$(NC)"
	@./$(NAME_PRT)

test_parentheses_shell: parentheses
	@echo "$(BLUE)=== TESTS SHELL - PARENTHÈSES PHASE 8.1 ===$(NC)"
	@if [ -f "test_paren.sh" ]; then \
		chmod +x test_paren.sh; \
		./test_paren.sh; \
	else \
		echo "$(RED)❌ Script test_paren.sh non trouvé$(NC)"; \
	fi

# Tests complets (C + Shell)
test_parentheses_full: test_parentheses_c test_parentheses_shell
	@echo "$(GREEN)✅ Tests parenthèses Phase 8.1 complets terminés !$(NC)"

# Test rapide - validation de base
test_phase_8: $(NAME_PRT)
	@echo "$(YELLOW)🔍 Test Phase 8 - Parenthèses et Groupes$(NC)"
	@echo "(echo hello)" | timeout 5 ./$(NAME_PRT) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Phase 8 OK$(NC)" || echo "$(RED)❌ Phase 8 FAIL$(NC)"

# Test de validation syntaxique
test_paren_syntax: $(NAME_PRT)
	@echo "$(YELLOW)🔍 Test syntaxe parenthèses$(NC)"
	@echo "(echo test" | timeout 5 ./$(NAME_PRT) 2>&1 | grep -q "error" && \
		echo "$(GREEN)✅ Validation syntaxe OK$(NC)" || echo "$(RED)❌ Validation syntaxe FAIL$(NC)"

# ********************************** HELP *********************************** #

help_parentheses:
	@echo "$(BLUE)📚 AIDE - TARGETS PARENTHÈSES PHASE 8.1 DISPONIBLES:$(NC)"
	@echo "  $(YELLOW)parentheses$(NC)              - Compile le module parenthèses"
	@echo "  $(YELLOW)test_parentheses_auto$(NC)    - Lance tous les tests (C + shell)"
	@echo "  $(YELLOW)test_parentheses_c$(NC)       - Tests C uniquement"
	@echo "  $(YELLOW)test_parentheses_shell$(NC)   - Tests shell uniquement"
	@echo "  $(YELLOW)test_parentheses_full$(NC)    - Tests complets"
	@echo "  $(YELLOW)test_phase_8$(NC)             - Test rapide Phase 8"
	@echo "  $(YELLOW)test_paren_syntax$(NC)        - Test validation syntaxique"
	@echo "  $(YELLOW)clean_parentheses$(NC)        - Nettoie les objets"
	@echo "  $(YELLOW)fclean_parentheses$(NC)       - Nettoie tout"
	@echo "  $(YELLOW)re_parentheses$(NC)           - Recompile tout"
	@echo "  $(YELLOW)help_parentheses$(NC)         - Affiche cette aide"
	@echo ""
	@echo "$(GREEN)💡 EXEMPLES D'UTILISATION:$(NC)"
	@echo "  make parentheses && make test_parentheses_auto    # Compile et teste tout"
	@echo "  make test_phase_8                                 # Test rapide phase 8"
	@echo "  make test_paren_syntax                            # Validation syntaxique"

# ********************************* PHONY *********************************** #

.PHONY: parentheses clean_parentheses fclean_parentheses re_parentheses \
        test_parentheses_auto test_parentheses_c test_parentheses_shell \
        test_parentheses_full test_phase_8 test_paren_syntax \
        help_parentheses dir_parentheses

# ******************************** ALIASES ********************************** #

# Aliases pour compatibilité
paren: parentheses
test_paren: test_parentheses_auto
clean_paren: clean_parentheses
fclean_paren: fclean_parentheses

# ********************************* BANNER *********************************** #

BAN_PRT = \
" **********************************************************" "\n" \
"*$(Y)  _____        _____   ______ _   _ ______ _____  _____ $(O)*" "\n" \
"*$(Y) |  __ \ /\   |  __ \ |  ____| \ | |  ____|  __ \|  __ \$(O)*" "\n" \
"*$(Y) | |__) /  \  | |__) || |__  |  \| | |__  | |__) | |__) |$(O)*" "\n" \
"*$(Y) |  ___/ /\ \ |  _  / |  __| | . \` |  __| |  ___/|  _  / $(O)*" "\n" \
"*$(Y) | |  / ____ \| | \ \ | |____| |\  | |____| |    | | \ \ $(O)*" "\n" \
"*$(Y) |_| /_/    \_\_|  \_\|______|_| \_|______|_|    |_|  \_\$(O)*" "\n" \
"*$(V) PARENTHÈSES PHASE 8.1 - Tests Intégrés              $(O)*" "\n" \
"*$(V) Groupes () -> Validation -> Parsing -> AST NODE_GROUP$(O)*" "\n" \
"*$(V) Made by jjorda - $(shell date +%d/%m/%Y)             $(O)*" "\n" \
" **********************************************************"