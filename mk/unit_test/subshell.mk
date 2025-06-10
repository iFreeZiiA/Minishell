# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    subshell.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/29 20:15:00 by jjorda            #+#    #+#              #
#    Updated: 2025/06/01 14:35:31 by jjorda           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_SS = subshell_ut

# Correction 1: Utiliser DIR_UT avec un slash final pour cohérence
DIR_SS = $(DIR_UT)/
SBS = $(MAN)/parser/lexing/subshell/

# Correction 2: Ajouter les dépendances nécessaires (lexer + setup)
SRC_SBS = $(SBS)subshell_parse.c $(SBS)subshell_utils.c $(SBS)subshell_validation.c
UT_SBS = $(SBS)main_test.c $(SRC_SBS) $(SRC_LXR) $(SRC_SUP)

OBJ_SBS = $(patsubst %.c, $(DIR_SS)%.o, $(UT_SBS))

subshell: $(LIB) $(NAME_SS)

$(NAME_SS): $(OBJ_SBS)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_SBS) $(LIBS)
	@$(PRINT) $(BAN_SS)

# Correction 3: Créer tous les répertoires nécessaires
dir_subshell: dir_lexer dir_setup
	@mkdir -p $(DIR_SS)$(SBS)

# Correction 4: Règles de compilation pour tous les répertoires
$(DIR_SS)$(SBS)%.o: $(SBS)%.c | dir_subshell
	@$(CC) $(CFLAGS) -c $< -o $@

# Règles supplémentaires pour les dépendances (lexer)
$(DIR_SS)$(LEX)%.o: $(LEX)%.c | dir_subshell
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Règles supplémentaires pour les dépendances (setup)
$(DIR_SS)$(SUP)%.o: $(SUP)%.c | dir_subshell
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Règles pour les dépendances (expansion)
$(DIR_SS)$(EXP)%.o: $(EXP)%.c | dir_subshell
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

BAN_SS = \
"**********************************************" "\n" \
"*$(Y)    _____ _    _ ____   _____ _    _ ______ _      _      $(O)*" "\n" \
"*$(Y)   / ____| |  | |  _ \ / ____| |  | |  ____| |    | |     $(O)*" "\n" \
"*$(Y)  | (___ | |  | | |_) | (___ | |__| | |__  | |    | |     $(O)*" "\n" \
"*$(Y)   \___ \| |  | |  _ < \___ \|  __  |  __| | |    | |     $(O)*" "\n" \
"*$(Y)   ____) | |__| | |_) |____) | |  | | |____| |____| |____ $(O)*" "\n" \
"*$(Y)  |_____/ \____/|____/|_____/|_|  |_|______|______|______| $(O)*" "\n" \
"*$(V) SUBSHELL PARSING - Made by jjorda                        $(O)*" "\n" \
"*$(V) Parentheses and subshell support                         $(O)*" "\n" \
"*$(V) Started : 04/04/2025                                     $(O)*" "\n" \
"**********************************************"