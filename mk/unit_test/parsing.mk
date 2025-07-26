# ********************************* PHONY *********************************** #

.PHONY: parser clean_parsing_5_1 fclean_parsing_5_1 re_parsing_5_1 \
        test_parsing test_parsing_c test_parsing_shell test_parsing_5_1 \
        test_phase_2 test_phase_3 test_phase_4 test_phase_5 \
        test_regression help_parsing dir_parsing_5_1

# ******************************** ALIASES ********************************** #

# Aliases pour compatibilité avec l'ancien makefile
parsing: parser
parsing_5_1: parser
clean_parsing: clean_parsing_5_1
fclean_parsing: fclean_parsing_5_1
re_parsing: re_parsing_5_1# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    parsing.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/20 17:45:00 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ********************************* PARSING ********************************** #

NAME_PS = parser_phase_5_1
NAME_UTIL = parser_ut

DIR_PS	= $(DIR_UT)
PSR		= $(MAN)/parser/parsing/
PSR_U	= $(PSR)utils/

# Sources du module parsing Phase 5.1
SRC_PSR	= $(PSR)parser.c $(PSR_U)ast.c $(PSR_U)token.c

# Unit test parsing Phase 5.1 - UTILISE NOTRE NOUVEAU main_test.c
UT_PSR	= $(PSR)main_test.c $(SRC_PSR) $(SRC_LXR) $(SRC_SUP)

# Utilitaire pour tests shell
UT_UTIL = $(PSR)parser_util.c $(SRC_PSR) $(SRC_LXR) $(SRC_SUP)

OBJ_PSR	= $(patsubst %.c, $(DIR_PS)%.o, $(UT_PSR))
OBJ_UTIL = $(patsubst %.c, $(DIR_PS)%.o, $(UT_UTIL))

# ********************************** RULES *********************************** #

parser: $(LIB) $(NAME_PS) $(NAME_UTIL)

$(NAME_PS): $(OBJ_PSR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PSR) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_PS)

$(NAME_UTIL): $(OBJ_UTIL)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_UTIL) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@echo "$(GREEN)✅ Utilitaire parser_ut créé !$(NC)"

dir_parsing_5_1:
	@mkdir -p $(DIR_PS)$(PSR)
	@mkdir -p $(DIR_PS)$(PSR_U)
	@mkdir -p $(DIR_PS)$(LEX)
	@mkdir -p $(DIR_PS)$(EXP)
	@mkdir -p $(DIR_PS)$(SUP)

$(DIR_PS)$(PSR)%.o: $(PSR)%.c | dir_parsing_5_1
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PS)$(LEX)%.o: $(LEX)%.c | dir_parsing_5_1
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PS)$(EXP)%.o: $(EXP)%.c | dir_parsing_5_1
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PS)$(SUP)%.o: $(SUP)%.c | dir_parsing_5_1
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_parsing_5_1:
	@$(RM) -f $(OBJ_PSR) $(OBJ_UTIL)

fclean_parsing_5_1: clean_parsing_5_1
	@$(RM) -f $(NAME_PS) $(NAME_UTIL)

re_parsing_5_1: fclean_parsing_5_1 parser

# ********************************* TESTS *********************************** #

# Tests automatisés C
test_parsing_c: $(NAME_PS)
	@echo "$(BLUE)=== TESTS C - PHASES 2-5 ===$(NC)"
	@./$(NAME_PS)

# Tests automatisés shell (nécessite le script test_parser.sh)
test_parsing_shell: $(NAME_PS)
	@echo "$(BLUE)=== TESTS SHELL - PHASES 2-5 ===$(NC)"
	@if [ -f "test_parser.sh" ]; then \
		chmod +x test_parser.sh; \
		./test_parser.sh; \
	else \
		echo "$(RED)❌ Script test_parser.sh non trouvé$(NC)"; \
		echo "$(YELLOW)💡 Créez le script test_parser.sh pour les tests shell$(NC)"; \
	fi

# Tests complets (C + Shell)
test_parsing: test_parsing_c test_parsing_shell
	@echo "$(GREEN)✅ Tests parsing complets terminés !$(NC)"

# Tests par phase individuelle
test_phase_2: $(NAME_PS)
	@echo "$(YELLOW)🔍 Test Phase 2 - Tokenisation$(NC)"
	@echo "echo hello world" | timeout 5 ./$(NAME_PS) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Phase 2 OK$(NC)" || echo "$(RED)❌ Phase 2 FAIL$(NC)"

test_phase_3: $(NAME_PS)
	@echo "$(YELLOW)🔍 Test Phase 3 - Quotes & Expansion$(NC)"
	@echo "echo \"test quotes\"" | timeout 5 ./$(NAME_PS) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Phase 3 OK$(NC)" || echo "$(RED)❌ Phase 3 FAIL$(NC)"

test_phase_4: $(NAME_PS)
	@echo "$(YELLOW)🔍 Test Phase 4 - Opérateurs$(NC)"
	@echo "echo a && echo b" | timeout 5 ./$(NAME_PS) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Phase 4 OK$(NC)" || echo "$(RED)❌ Phase 4 FAIL$(NC)"

test_phase_5: $(NAME_PS)
	@echo "$(YELLOW)🔍 Test Phase 5 - Parser/AST$(NC)"
	@echo "echo hello" | timeout 5 ./$(NAME_PS) >/dev/null 2>&1 && \
		echo "$(GREEN)✅ Phase 5 OK$(NC)" || echo "$(RED)❌ Phase 5 FAIL$(NC)"

# Tests de régression
test_regression: parser
	@echo "$(PURPLE)🔄 Tests de régression...$(NC)"
	@$(MAKE) test_phase_2
	@$(MAKE) test_phase_3
	@$(MAKE) test_phase_4
	@$(MAKE) test_phase_5

# Test original pour compatibilité
test_parsing_5_1: test_parsing

# ********************************* BANNER *********************************** #

BAN_PS = \
" **********************************************************" "\n" \
"*$(Y)   _____ _    _ ____   _____ _    _ ______ _      _       $(O)*" "\n" \
"*$(Y)  / ____| |  | |  _ \ / ____| |  | |  ____| |    | |      $(O)*" "\n" \
"*$(Y) | (___ | |  | | |_) | (___ | |__| | |__  | |    | |      $(O)*" "\n" \
"*$(Y)  \___ \| |  | |  _ < \___ \|  __  |  __| | |    | |      $(O)*" "\n" \
"*$(Y)  ____) | |__| | |_) |____) | |  | | |____| |____| |____  $(O)*" "\n" \
"*$(Y) |_____/ \____/|____/|_____/|_|  |_|______|______|______| $(O)*" "\n" \
"*$(V) PARSING PHASES 2-5 - Tests Intégrés                     $(O)*" "\n" \
"*$(V) Tokenisation -> Quotes -> Opérateurs -> Parser/AST      $(O)*" "\n" \
"*$(V) Made by student - $(shell date +%d/%m/%Y)               $(O)*" "\n" \
" **********************************************************"

# ********************************* HELP *********************************** #

help_parsing:
	@echo "$(BLUE)📚 AIDE - TARGETS PARSING DISPONIBLES:$(NC)"
	@echo "  $(YELLOW)parser$(NC)               - Compile tous les tests"
	@echo "  $(YELLOW)test_parsing$(NC)         - Lance tous les tests (C + shell)"
	@echo "  $(YELLOW)test_parsing_c$(NC)       - Tests C uniquement"
	@echo "  $(YELLOW)test_parsing_shell$(NC)   - Tests shell uniquement"
	@echo "  $(YELLOW)test_phase_2$(NC)         - Test Phase 2 (Tokenisation)"
	@echo "  $(YELLOW)test_phase_3$(NC)         - Test Phase 3 (Quotes/Expansion)"
	@echo "  $(YELLOW)test_phase_4$(NC)         - Test Phase 4 (Opérateurs)"
	@echo "  $(YELLOW)test_phase_5$(NC)         - Test Phase 5 (Parser/AST)"
	@echo "  $(YELLOW)test_regression$(NC)      - Tests de régression (toutes phases)"
	@echo "  $(YELLOW)clean_parsing$(NC)        - Nettoie les objets"
	@echo "  $(YELLOW)fclean_parsing$(NC)       - Nettoie tout"
	@echo "  $(YELLOW)help_parsing$(NC)         - Affiche cette aide"
	@echo ""
	@echo "$(GREEN)💡 EXEMPLES D'UTILISATION:$(NC)"
	@echo "  make parser && make test_parsing    # Compile et teste tout"
	@echo "  make test_phase_2                   # Test rapide phase 2"
	@echo "  make test_regression                # Vérification complète"