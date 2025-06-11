# ********************************** PARSING ********************************** #

NAME_PR = parsing_ut

DIR_PR = $(DIR_UT)/
PRR = $(MAN)/parser/parsing/

# ********************************** SOURCES ********************************** #

# Refactored parsing source files following 42 norms
SRC_PRR_NEW = $(PRR)ft_parsing.c $(PRR)ast_creation.c $(PRR)expression_parsing.c \
			$(PRR)operator_utils.c $(PRR)command_parsing.c $(PRR)parsing_utils.c \
			$(PRR)ast_display.c

# Test files and dependencies
UT_PRR = $(PRR)main_test.c $(SRC_PRR_NEW) $(SRC_LXR) $(SRC_SUP)

# ********************************** OBJECTS ********************************** #

OBJ_PRR = $(patsubst %.c, $(DIR_PR)%.o, $(UT_PRR))

# ********************************** TARGETS ********************************** #

parsing: $(LIB) $(NAME_PR)

$(NAME_PR): $(OBJ_PRR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PRR) $(LIBS)
	@$(PRINT) $(BAN_PR)

# ******************************** DIRECTORIES ******************************* #

dir_parsing: dir_lexer dir_setup
	@mkdir -p $(DIR_PR)$(PRR)

# ******************************* COMPILATION ******************************* #

# Parsing module objects
$(DIR_PR)$(PRR)%.o: $(PRR)%.c | dir_parsing
	@$(CC) $(CFLAGS) -c $< -o $@

# Lexer dependencies
$(DIR_PR)$(LEX)%.o: $(LEX)%.c | dir_parsing
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Setup dependencies
$(DIR_PR)$(SUP)%.o: $(SUP)%.c | dir_parsing
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Expansion dependencies
$(DIR_PR)$(EXP)%.o: $(EXP)%.c | dir_parsing
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# ********************************** BANNER *********************************** #

BAN_PR = \
"**********************************************" "\n" \
"*$(Y)   ______      ______     _    _______       $(O)*" "\n" \
"*$(Y)  (_____ \ /\ (_____ \   | |  (_______)     $(O)*" "\n" \
"*$(Y)   _____) )  \ _____) )   \ \  _____        $(O)*" "\n" \
"*$(Y)  |  ____/ /\ \_____ (     \ \|  ___)       $(O)*" "\n" \
"*$(Y)  | |   | |__| |    | |_____) ) |_____      $(O)*" "\n" \
"*$(Y)  |_|   |______|    |_|______/|_______)     $(O)*" "\n" \
"*$(V) Made by : alearroy / jjorda                $(O)*" "\n" \
"*$(V) Started : 04/04/2025                       $(O)*" "\n" \
"*$(V) Finished :                                 $(O)*" "\n" \
"**********************************************"