# ******************************** DIRECTORIES ******************************* #

dir_parsing:
	@mkdir -p $(DIR_PSI)$(PSR)
	@mkdir -p $(DIR_PSI)$(PSR_U)
# 	@mkdir -p $(DIR_PSI)$(PSR_OPS)
	@mkdir -p $(DIR_PSI)$(PSR_PIP)
	@mkdir -p $(DIR_PSI)$(PSR_LOG)
	@mkdir -p $(DIR_PSI)$(PSR_PAR)
	@mkdir -p $(DIR_PSI)$(PSR_RED)
	@mkdir -p $(DIR_PSI)$(PSR_HER)
	@mkdir -p $(DIR_PSI)$(PSR_WLD)
	@mkdir -p $(DIR_PSI)$(PSR_VAL)
	@mkdir -p $(DIR_PSI)$(LEX)
	@mkdir -p $(DIR_PSI)$(EXP)
	@mkdir -p $(DIR_PSI)$(SUP)
	@mkdir -p $(DIR_PSI)$(CUP)

# ******************************** COMPILATION ******************************* #

$(DIR_PSI)$(PSR)%.o: $(PSR)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_U)%.o: $(PSR_U)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

# $(DIR_PSI)$(PSR_OPS)%.o: $(PSR_OPS)%.c | dir_parsing
# 	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_PIP)%.o: $(PSR_PIP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_LOG)%.o: $(PSR_LOG)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_PAR)%.o: $(PSR_PAR)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_RED)%.o: $(PSR_RED)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_HER)%.o: $(PSR_HER)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_WLD)%.o: $(PSR_WLD)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(PSR_VAL)%.o: $(PSR_VAL)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(LEX)%.o: $(LEX)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(EXP)%.o: $(EXP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(SUP)%.o: $(SUP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PSI)$(CUP)%.o: $(CUP)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_parsing:
	@$(RM) -f $(OBJ_PSI_MAIN) $(OBJ_PSI_UTIL) $(OBJ_PSI_INTEGRATION)
	@echo "$(RED)✗ Objets parsing supprimés$(NC)"

fclean_parsing: clean_parsing
	@$(RM) -f $(NAME_PSI) $(NAME_PSI_UTIL) $(NAME_PSI_TEST)
	@echo "$(RED)✗ Exécutables parsing supprimés$(NC)"

re_parsing: fclean_parsing parsing

# ********************************* TESTS *********************************** #

# Tests automatisés C - toutes phases
test_parsing_c: $(NAME_PSI)
	@echo "$(BLUE)=== TESTS C - PARSING INTÉGRÉ ===$(NC)"
	@./$(NAME_PSI)

# Tests shell avec script
test_parsing_shell: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TESTS SHELL - PARSING INTÉGRÉ ===$(NC)"
	@if [ -f "test_parsing_integration.sh" ]; then \
		chmod +x test_parsing_integration.sh; \
		./test_parsing_integration.sh; \
	else \
		echo "$(YELLOW)Script test_parsing_integration.sh non trouvé$(NC)"; \
		echo "$(YELLOW)Lancement des tests manuels

# Noms des exécutables
NAME_PSI = parser_integration
NAME_PSI_UTIL = parser_integration_ut
NAME_PSI_TEST = integration_simple_test

DIR_PS = $(DIR_UT)
PSR_DIR	= $(MAN)/parser/
PSR		= $(PSR_DIR)parsing/
PSR_U	= $(PSR)utils/
# PSR_OPS	= $(PSR)operators/
PSR_PIP = $(PSR)pipes/
PSR_LOG = $(PSR)logical/
PSR_PAR = $(PSR)parentheses/
PSR_RED = $(PSR)redir/
PSR_HER = $(PSR)heredoc/
PSR_WLD = $(PSR)wildcard/
PSR_VAL = $(PSR)validate/

# ********************************* SOURCES ********************************** #

# Sources principales du parsing intégré (phases 2-11.4)
SRC_PSR_CORE = $(PSR)parser.c $(PSR)parse.c $(PSR_DIR)parser.c $(PSR_DIR)enhanced.c

# Sources utilitaires
SRC_PSR_UTILS = $(PSR_U)parser.c $(PSR_U)ast.c $(PSR_U)token.c $(PSR_U)logical.c

# Sources operators (Phase 4 + 6 + 7)
SRC_PSR_OPS = $(PSR_LOG)logical.c $(PSR_LOG)precedence.c

SRC_PSR_PIP = $(PSR_PIP)operators.c $(PSR_PIP)pipes.c

# Sources parentheses (Phase 8)
SRC_PSR_PAR = $(PSR_PAR)parentheses.c

# Sources redirections + heredoc (Phase 6.2 + 10)
SRC_PSR_RED = $(PSR_RED)redir.c $(PSR_RED)validate.c $(PSR_RED)input.c $(PSR_RED)output.c	\
	$(PSR_U)redir.c $(PSR_HER)heredoc.c

# Sources validation (Phase 11)
SRC_PSR_VAL = $(PSR_VAL)validate.c $(PSR_VAL)precedence.c $(PSR_VAL)ast_optimizer.c

# Sources wildcard (Phase 9) - si dans parsing
SRC_PSR_WLD = $(PSR_WLD)wildcard.c $(PSR_WLD)wc_files.c $(PSR_WLD)wc_match.c $(PSR_WLD)wc_tokens.c

# Assemblage de toutes les sources parsing
SRC_PSR_ALL = $(SRC_PSR_CORE) $(SRC_PSR_UTILS) $(SRC_PSR_OPS) $(SRC_PSR_PAR) $(SRC_PSR_RED)	\
	$(SRC_PSR_VAL) $(SRC_PSR_PIP)

# Sources avec dépendances (lexer + setup)
SRC_PSR_DEPS = $(SRC_PSR_ALL) $(SRC_LXR) $(SRC_SUP) $(SRC_CUP)

# Unit tests
UT_PSI_MAIN = $(PSR_DIR)simple_integration_test.c $(SRC_PSR_DEPS)
UT_PSI_UTIL = $(PSR)parser_util.c $(SRC_PSR_DEPS)
UT_PSI_INTEGRATION = $(PSR_DIR)main_test.c $(SRC_PSR_DEPS)

# ********************************* OBJECTS ********************************** #

OBJ_PSI_MAIN = $(patsubst %.c, $(DIR_PSI)%.o, $(UT_PSI_MAIN))
OBJ_PSI_UTIL = $(patsubst %.c, $(DIR_PSI)%.o, $(UT_PSI_UTIL))
OBJ_PSI_INTEGRATION = $(patsubst %.c, $(DIR_PSI)%.o, $(UT_PSI_INTEGRATION))

# ********************************** RULES *********************************** #

parsing: $(LIB) $(NAME_PSI) $(NAME_PSI_UTIL) $(NAME_PSI_TEST)
	@echo "$(GREEN)✅ Module parsing intégré compilé avec succès !$(NC)"
	@echo "$(YELLOW)Tests disponibles:$(NC)"
	@echo "  ./$(NAME_PSI)              - Tests unitaires principales"
	@echo "  ./$(NAME_PSI_UTIL)         - Utilitaire de test manuel"
	@echo "  ./$(NAME_PSI_TEST)         - Tests d'intégration simple"

$(NAME_PSI): $(OBJ_PSI_MAIN)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PSI_MAIN) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_PS)

$(NAME_PSI_UTIL): $(OBJ_PSI_UTIL)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PSI_UTIL) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@echo "$(GREEN)✅ Utilitaire parser_integration_ut créé !$(NC)"

$(NAME_PSI_TEST): $(OBJ_PSI_INTEGRATION)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PSI_INTEGRATION) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@echo "$(GREEN)✅ Tests d'intégration simple créés !$(NC)"

# Tests shell avec script
test_parsing_shell: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TESTS SHELL - PARSING INTÉGRÉ ===$(NC)"
	@if [ -f "test_parsing_integration.sh" ]; then \
		chmod +x test_parsing_integration.sh; \
		./test_parsing_integration.sh; \
	else \
		echo "$(YELLOW)Script test_parsing_integration.sh non trouvé$(NC)"; \
		echo "$(YELLOW)Lancement des tests manuels:$(NC)"; \
		./$(NAME_PSI_UTIL) "echo hello"; \
		./$(NAME_PSI_UTIL) "echo hello | cat"; \
		./$(NAME_PSI_UTIL) "true && echo success"; \
	fi

# Tests d'intégration massive
test_integration_massive: $(NAME_PSI_TEST)
	@echo "$(BLUE)=== TESTS INTÉGRATION MASSIVE ===$(NC)"
	@./$(NAME_PSI_TEST)

# Tests par phase
test_phase_2: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 2 - TOKENISATION ===$(NC)"
	@./$(NAME_PSI_UTIL) "echo hello world"

test_phase_3: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 3 - QUOTES/EXPANSION ===$(NC)"
	@./$(NAME_PSI_UTIL) "echo \"hello world\""

test_phase_4: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 4 - OPÉRATEURS ===$(NC)"
	@./$(NAME_PSI_UTIL) "echo hello > file"

test_phase_5: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 5 - PARSER/AST ===$(NC)"
	@./$(NAME_PSI_UTIL) "echo hello"

test_phase_6: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 6 - PIPES ===$(NC)"
	@./$(NAME_PSI_UTIL) "echo hello | cat"

test_phase_7: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 7 - OPÉRATEURS LOGIQUES ===$(NC)"
	@./$(NAME_PSI_UTIL) "true && echo success"

test_phase_8: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 8 - PARENTHÈSES ===$(NC)"
	@./$(NAME_PSI_UTIL) "(echo hello)"

test_phase_9: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 9 - WILDCARDS ===$(NC)"
	@./$(NAME_PSI_UTIL) "echo *.c"

test_phase_10: $(NAME_PSI_UTIL)
	@echo "$(BLUE)=== TEST PHASE 10 - HEREDOC ===$(NC)"
	@echo "$(YELLOW)Tests heredoc complexes - voir test_heredoc.sh$(NC)"

test_phase_11: $(NAME_PSI_TEST)
	@echo "$(BLUE)=== TEST PHASE 11 - INTÉGRATION ===$(NC)"
	@./$(NAME_PSI_TEST)

# Tests de régression complets
test_regression: parsing
	@echo "$(BLUE)=== TESTS DE RÉGRESSION - TOUTES PHASES ===$(NC)"
	@make test_phase_2
	@make test_phase_3
	@make test_phase_4
	@make test_phase_5
	@make test_phase_6
	@make test_phase_7
	@make test_phase_8
	@make test_phase_9
	@make test_phase_11
	@echo "$(GREEN)✅ Tests de régression terminés$(NC)"

# Tous les tests
test_parsing: test_parsing_c test_parsing_shell test_integration_massive
	@echo "$(GREEN)✅ Tous les tests parsing terminés$(NC)"

# ********************************* BANNER *********************************** #

BAN_PS = \
" **********************************************************" "\n" \
"*$(Y)  ____  _____  _____ _  _ _____ _  _  _____             $(O)*" "\n" \
"*$(Y) |  _ \|  _  ||  _  | || ||  _  | \| ||   __|            $(O)*" "\n" \
"*$(Y) |   __| |_| ||     ||    ||  _  | \| ||  |  |           $(O)*" "\n" \
"*$(Y) |__|  |_____||__|__||_||_||_| |_|_|\_||_____|           $(O)*" "\n" \
"*$(V) PARSING INTÉGRÉ - Phases 2-11.4                       $(O)*" "\n" \
"*$(V) Lexing -> Expansion -> Parsing -> AST                 $(O)*" "\n" \
"*$(V) Made by jjorda - $(shell date +%d/%m/%Y)              $(O)*" "\n" \
" **********************************************************"

# ********************************* HELP *********************************** #

help_parsing:
	@echo "$(BLUE)📚 AIDE - TARGETS PARSING DISPONIBLES:$(NC)"
	@echo ""
	@echo "$(YELLOW)🔧 COMPILATION:$(NC)"
	@echo "  $(GREEN)parsing$(NC)               - Compile tous les tests parsing"
	@echo "  $(GREEN)clean_parsing$(NC)         - Nettoie les objets"
	@echo "  $(GREEN)fclean_parsing$(NC)        - Nettoie tout"
	@echo "  $(GREEN)re_parsing$(NC)            - Recompile tout"
	@echo ""
	@echo "$(YELLOW)🧪 TESTS GLOBAUX:$(NC)"
	@echo "  $(GREEN)test_parsing$(NC)          - Lance tous les tests (C + shell + intégration)"
	@echo "  $(GREEN)test_parsing_c$(NC)        - Tests C uniquement"
	@echo "  $(GREEN)test_parsing_shell$(NC)    - Tests shell uniquement"
	@echo "  $(GREEN)test_integration_massive$(NC) - Tests d'intégration massive"
	@echo "  $(GREEN)test_regression$(NC)       - Tests de régression (toutes phases)"
	@echo ""
	@echo "$(YELLOW)🎯 TESTS PAR PHASE:$(NC)"
	@echo "  $(GREEN)test_phase_2$(NC)          - Phase 2: Tokenisation"
	@echo "  $(GREEN)test_phase_3$(NC)          - Phase 3: Quotes/Expansion"
	@echo "  $(GREEN)test_phase_4$(NC)          - Phase 4: Opérateurs"
	@echo "  $(GREEN)test_phase_5$(NC)          - Phase 5: Parser/AST"
	@echo "  $(GREEN)test_phase_6$(NC)          - Phase 6: Pipes"
	@echo "  $(GREEN)test_phase_7$(NC)          - Phase 7: Opérateurs logiques"
	@echo "  $(GREEN)test_phase_8$(NC)          - Phase 8: Parenthèses"
	@echo "  $(GREEN)test_phase_9$(NC)          - Phase 9: Wildcards"
	@echo "  $(GREEN)test_phase_10$(NC)         - Phase 10: Heredoc"
	@echo "  $(GREEN)test_phase_11$(NC)         - Phase 11: Intégration"
	@echo ""
	@echo "$(YELLOW)💡 EXEMPLES D'UTILISATION:$(NC)"
	@echo "  make parsing && make test_parsing    # Compile et teste tout"
	@echo "  make test_phase_6                    # Test rapide pipes"
	@echo "  make test_regression                 # Vérification complète"
	@echo "  make test_integration_massive        # Tests complexes"

# ********************************* PHONY *********************************** #

.PHONY: parsing clean_parsing fclean_parsing re_parsing \
        test_parsing test_parsing_c test_parsing_shell test_integration_massive \
        test_phase_2_parsing test_phase_3_parsing test_phase_4_parsing test_phase_5_parsing test_phase_6_parsing \
        test_phase_7_parsing test_phase_8_parsing test_phase_9_parsing test_phase_10_parsing test_phase_11_parsing \
        test_regression help_parsing dir_parsing

# ******************************** ALIASES ********************************** #

# Aliases pour compatibilité avec l'ancien makefile
parser: parsing
parser_ut: $(NAME_PSI_UTIL)
integration_test: $(NAME_PSI_TEST)
clean_parser: clean_parsing
fclean_parser: fclean_parsing
re_parser: re_parsing

# Redirection des targets en conflit vers les versions spécifiques
test_phase_2_parsing: test_phase_2
test_phase_3_parsing: test_phase_3
test_phase_4_parsing: test_phase_4
test_phase_5_parsing: test_phase_5
test_phase_6_parsing: test_phase_6
test_phase_7_parsing: test_phase_7
test_phase_8_parsing: test_phase_8
test_phase_9_parsing: test_phase_9
test_phase_10_parsing: test_phase_10
test_phase_11_parsing: test_phase_11
