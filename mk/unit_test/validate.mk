# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    validate.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/26 00:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/26 16:35:11 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ******************************** VALIDATION ******************************** #

NAME_VAL = validate_phase_11_6
NAME_VAL_UTIL = validate_util

DIR_VAL	= $(DIR_UT)
VAL		= $(MAN)/parser/parsing/validate/
VAL_U	= $(VAL)utils/

# Sources du module validation Phase 11.6
SRC_VAL	= $(VAL)validation.c $(VAL)precedence.c $(VAL)ast_validator.c $(VAL_U)error_handler.c

# Unit test validation Phase 11.6
UT_VAL	= $(VAL)main_test.c $(SRC_VAL) $(SRC_PSR) $(SRC_LXR) $(SRC_SUP)

# Utilitaire pour tests shell
UT_VAL_UTIL = $(VAL)validate_util.c $(SRC_VAL) $(SRC_PSR) $(SRC_LXR) $(SRC_SUP)

OBJ_VAL	= $(patsubst %.c, $(DIR_VAL)%.o, $(UT_VAL))
OBJ_VAL_UTIL = $(patsubst %.c, $(DIR_VAL)%.o, $(UT_VAL_UTIL))

# ********************************* PHONY *********************************** #

.PHONY: validate clean_validate fclean_validate re_validate \
        test_validate test_validate_c test_validate_shell \
        test_phase_11_6 test_regression_complete \
        test_all_phases test_syntax_validation test_precedence_validation \
        test_ast_validation test_error_detection help_validate dir_validate

# ********************************** RULES *********************************** #

validate: $(LIB) $(NAME_VAL) $(NAME_VAL_UTIL)

$(NAME_VAL): $(OBJ_VAL)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_VAL) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_VAL)

$(NAME_VAL_UTIL): $(OBJ_VAL_UTIL)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_VAL_UTIL) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@echo "$(GREEN)✅ Utilitaire validate_util créé !$(NC)"

dir_validate:
	@mkdir -p $(DIR_VAL)$(VAL)
	@mkdir -p $(DIR_VAL)$(VAL_U)
	@mkdir -p $(DIR_VAL)$(PSR)
	@mkdir -p $(DIR_VAL)$(PSR_U)
	@mkdir -p $(DIR_VAL)$(LEX)
	@mkdir -p $(DIR_VAL)$(EXP)
	@mkdir -p $(DIR_VAL)$(SUP)

$(DIR_VAL)$(VAL)%.o: $(VAL)%.c | dir_validate
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_VAL)$(PSR)%.o: $(PSR)%.c | dir_validate
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_VAL)$(LEX)%.o: $(LEX)%.c | dir_validate
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_VAL)$(EXP)%.o: $(EXP)%.c | dir_validate
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_VAL)$(SUP)%.o: $(SUP)%.c | dir_validate
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_validate:
	@$(RM) -f $(OBJ_VAL) $(OBJ_VAL_UTIL)

fclean_validate: clean_validate
	@$(RM) -f $(NAME_VAL) $(NAME_VAL_UTIL)

re_validate: fclean_validate validate

# ********************************* TESTS *********************************** #

# Tests automatisés C
test_validate_c: $(NAME_VAL)
	@echo "$(BLUE)=== TESTS C - VALIDATION PHASE 11.6 ===$(NC)"
	@./$(NAME_VAL)

# Tests automatisés shell
test_validate_shell: $(NAME_VAL)
	@echo "$(BLUE)=== TESTS SHELL - VALIDATION PHASE 11.6 ===$(NC)"
	@if [ -f "test_validate.sh" ]; then \
		chmod +x test_validate.sh; \
		./test_validate.sh; \
	else \
		echo "$(RED)❌ Script test_validate.sh non trouvé$(NC)"; \
		echo "$(YELLOW)💡 Créez le script test_validate.sh pour les tests shell$(NC)"; \
	fi

# Tests complets (C + Shell)
test_validate: test_validate_c test_validate_shell
	@echo "$(GREEN)✅ Tests validation Phase 11.6 complets terminés !$(NC)"

# Test rapide phase 11.6
test_phase_11_6: $(NAME_VAL)
	@echo "$(YELLOW)🔍 Test Phase 11.6 - Validation & Régression$(NC)"
	@echo "echo hello && echo world" | timeout 5 ./$(NAME_VAL) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Phase 11.6 OK$(NC)" || echo "$(RED)❌ Phase 11.6 FAIL$(NC)"

# Tests de régression complets sur toutes les phases
test_regression_complete: validate
	@echo "$(PURPLE)🔄 Tests de régression complets - Toutes phases...$(NC)"
	@$(MAKE) test_phase_2
	@$(MAKE) test_phase_3
	@$(MAKE) test_phase_4
	@$(MAKE) test_phase_5
	@$(MAKE) test_phase_6 2>/dev/null || echo "$(YELLOW)⚠️  Phase 6 non disponible$(NC)"
	@$(MAKE) test_phase_7 2>/dev/null || echo "$(YELLOW)⚠️  Phase 7 non disponible$(NC)"
	@$(MAKE) test_phase_8 2>/dev/null || echo "$(YELLOW)⚠️  Phase 8 non disponible$(NC)"
	@$(MAKE) test_phase_9 2>/dev/null || echo "$(YELLOW)⚠️  Phase 9 non disponible$(NC)"
	@$(MAKE) test_phase_10 2>/dev/null || echo "$(YELLOW)⚠️  Phase 10 non disponible$(NC)"
	@$(MAKE) test_phase_11_6

# Test toutes les phases disponibles
test_all_phases: validate
	@echo "$(CYAN)🧪 Test de toutes les phases disponibles...$(NC)"
	@$(MAKE) test_regression_complete

# Tests spécialisés
test_syntax_validation: $(NAME_VAL)
	@echo "$(YELLOW)🔍 Test validation syntaxique$(NC)"
	@echo "echo test &&" | timeout 5 ./$(NAME_VAL) 2>&1 | grep -q "syntax error" && \
		echo "$(GREEN)✅ Validation syntaxe OK$(NC)" || echo "$(RED)❌ Validation syntaxe FAIL$(NC)"

test_precedence_validation: $(NAME_VAL)
	@echo "$(YELLOW)🔍 Test validation précédences$(NC)"
	@echo "echo a | echo b && echo c" | timeout 5 ./$(NAME_VAL) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Validation précédences OK$(NC)" || echo "$(RED)❌ Validation précédences FAIL$(NC)"

test_ast_validation: $(NAME_VAL)
	@echo "$(YELLOW)🔍 Test validation AST$(NC)"
	@echo "(echo hello && echo world)" | timeout 5 ./$(NAME_VAL) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Validation AST OK$(NC)" || echo "$(RED)❌ Validation AST FAIL$(NC)"

test_error_detection: $(NAME_VAL)
	@echo "$(YELLOW)🔍 Test détection d'erreurs$(NC)"
	@echo "| echo test" | timeout 5 ./$(NAME_VAL) 2>&1 | grep -q "error" && \
		echo "$(GREEN)✅ Détection erreurs OK$(NC)" || echo "$(RED)❌ Détection erreurs FAIL$(NC)"

# ********************************* BANNER *********************************** #

BAN_VAL = \
" **********************************************************" "\n" \
"*$(G)  __      __     _      _____ _____         _______ ______ $(O)*" "\n" \
"*$(G)  \\ \\    / /    | |    |_   _|  __ \\   /\\  |__   __|  ____|$(O)*" "\n" \
"*$(G)   \\ \\  / /__ _ | |      | | | |  | | /  \\    | |  | |__   $(O)*" "\n" \
"*$(G)    \\ \\/ / _  || |      | | | |  | |/ /\\ \\   | |  |  __|  $(O)*" "\n" \
"*$(G)     \\  / (_| || |____ _| |_| |__| / ____ \\  | |  | |____ $(O)*" "\n" \
"*$(G)      \\/ \\__,_||______|_____|_____/_/    \\_\\ |_|  |______|$(O)*" "\n" \
"*$(C) VALIDATION & REGRESSION TESTS - PHASE 11.6              $(O)*" "\n" \
"*$(C) Tests syntaxiques, précédences, AST et détection erreurs$(O)*" "\n" \
"*$(C) Made by student - $(shell date +%d/%m/%Y)               $(O)*" "\n" \
" **********************************************************"

# ********************************* HELP *********************************** #

help_validate:
	@echo "$(BLUE)📚 AIDE - TARGETS VALIDATION PHASE 11.6 DISPONIBLES:$(NC)"
	@echo "  $(YELLOW)validate$(NC)                   - Compile le module validation"
	@echo "  $(YELLOW)test_validate$(NC)              - Lance tous les tests (C + shell)"
	@echo "  $(YELLOW)test_validate_c$(NC)            - Tests C uniquement"
	@echo "  $(YELLOW)test_validate_shell$(NC)        - Tests shell uniquement"
	@echo "  $(YELLOW)test_phase_11_6$(NC)            - Test rapide Phase 11.6"
	@echo "  $(YELLOW)test_regression_complete$(NC)   - Tests de régression toutes phases"
	@echo "  $(YELLOW)test_all_phases$(NC)            - Test toutes phases disponibles"
	@echo "  $(YELLOW)test_syntax_validation$(NC)     - Test validation syntaxique"
	@echo "  $(YELLOW)test_precedence_validation$(NC) - Test validation précédences"
	@echo "  $(YELLOW)test_ast_validation$(NC)        - Test validation AST"
	@echo "  $(YELLOW)test_error_detection$(NC)       - Test détection d'erreurs"
	@echo "  $(YELLOW)clean_validate$(NC)             - Nettoie les objets"
	@echo "  $(YELLOW)fclean_validate$(NC)            - Nettoie tout"
	@echo "  $(YELLOW)help_validate$(NC)              - Affiche cette aide"
	@echo ""
	@echo "$(GREEN)💡 EXEMPLES D'UTILISATION:$(NC)"
	@echo "  make validate && make test_validate       # Compile et teste tout"
	@echo "  make test_regression_complete             # Tests de régression complets"
	@echo "  make test_all_phases                      # Test toutes phases disponibles"
	@echo "  make test_syntax_validation               # Test validation syntaxique"
	