# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    parsing.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/19 00:00:00 by jjorda            #+#    #+#              #
#    Updated: 2025/07/20 16:24:11 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ********************************* PARSING ********************************** #

NAME_PS = parser_phase_5_1

DIR_PS	= $(DIR_UT)
PSR		= $(MAN)/parser/parsing/
PSR_U	= $(PSR)utils/

# Sources du module parsing Phase 5.1
SRC_PSR	= $(PSR)parser.c $(PSR_U)ast.c $(PSR_U)token.c

# Unit test parsing Phase 5.1
UT_PSR	= $(PSR)simple_test.c $(SRC_PSR) $(SRC_SUP)

OBJ_PSR	= $(patsubst %.c, $(DIR_PS)%.o, $(UT_PSR))

# ********************************** RULES *********************************** #

parser: $(LIB) $(NAME_PS)

$(NAME_PS): $(OBJ_PSR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PSR) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_PS)

dir_parsing_5_1:
	@mkdir -p $(DIR_PS)$(PSR)
	@mkdir -p $(DIR_PS)$(PSR_U)
	@mkdir -p $(DIR_PS)$(SUP)

$(DIR_PS)$(PSR)%.o: $(PSR)%.c | dir_parsing_5_1
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PS)$(SUP)%.o: $(SUP)%.c | dir_parsing_5_1
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************* CLEAN *********************************** #

clean_parsing_5_1:
	@$(RM) -f $(OBJ_PSR)

fclean_parsing_5_1: clean_parsing_5_1
	@$(RM) -f $(NAME_PS)

re_parsing_5_1: fclean_parsing_5_1 parsing_5_1

# ********************************* TESTS *********************************** #

test_parsing_5_1: parsing_5_1
	@echo "$(BLUE)=== TESTS PARSING PHASE 5.1 ===$(NC)"
	@echo "$(YELLOW)Test de base:$(NC)"
	@./$(NAME_PS)
	@echo "$(YELLOW)Test commande simple:$(NC)"
	@echo "echo hello" | ./$(NAME_PS)
	@echo "$(YELLOW)Test avec arguments:$(NC)"
	@echo "ls -la /tmp" | ./$(NAME_PS)

# ********************************* BANNER *********************************** #

BAN_PS = \
" **********************************************" "\n" \
"*$(Y)   _____      ______     _____  _______ ______    $(O)*" "\n" \
"*$(Y)  (_____ \ /\ (_____ \   / ____)(_______ _____ \   $(O)*" "\n" \
"*$(Y)   _____) )  \ _____) ) ( (___  _____   _____) )  $(O)*" "\n" \
"*$(Y)  |  ____/ /\ (_____ (   \___ \|  ___) |  __  /   $(O)*" "\n" \
"*$(Y)  | |   | |__| |    | |____) ) |_____  | |  \ \   $(O)*" "\n" \
"*$(Y)  |_|   |______|    |_(______/|_______)|_|   |_|  $(O)*" "\n" \
"*$(V) PARSING PHASE 5.1 - Made by jjorda              $(O)*" "\n" \
"*$(V) AST pour commandes simples                       $(O)*" "\n" \
"*$(V) Started : 19/07/2025                             $(O)*" "\n" \
"**********************************************"

# ********************************* PHONY *********************************** #

.PHONY: parsing_5_1 clean_parsing_5_1 fclean_parsing_5_1 re_parsing_5_1 test_parsing_5_1