NAME_PRT = parentheses_ut

DIR_PR	= $(DIR_UT)/
PRT		= $(MAN)/parser/parsing/parentheses/

UT_PRT	= $(PRT)main_test.c $(SRC_PRT) $(SRC_LXR) $(SRC_SUP)

OBJ_PRT = $(patsubst %.c, $(DIR_PR)%.o, $(UT_PRT))

parentheses: $(LIB) $(NAME_PRT)

$(NAME_PRT): $(OBJ_PRT)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PRT) $(LIBS)
	@$(PRINT) $(BAN_PR)

dir_parentheses: dir_lexer dir_setup
	@mkdir -p $(DIR_PR)$(PRT)

$(DIR_PR)$(PRT)%.o: $(PRT)%.c | dir_parentheses
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PR)$(LEX)%.o: $(LEX)%.c | dir_parentheses
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PR)$(SUP)%.o: $(SUP)%.c | dir_parentheses
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_PR)$(EXP)%.o: $(EXP)%.c | dir_parentheses
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

BAN_PR = \
"**********************************************" "\n" \
"*$(Y)   _____ _    _ ____   _____ _    _ ______ _      _       $(O)*" "\n" \
"*$(Y)  / ____| |  | |  _ \ / ____| |  | |  ____| |    | |      $(O)*" "\n" \
"*$(Y) | (___ | |  | | |_) | (___ | |__| | |__  | |    | |      $(O)*" "\n" \
"*$(Y)  \___ \| |  | |  _ < \___ \|  __  |  __| | |    | |      $(O)*" "\n" \
"*$(Y)  ____) | |__| | |_) |____) | |  | | |____| |____| |____  $(O)*" "\n" \
"*$(Y) |_____/ \____/|____/|_____/|_|  |_|______|______|______| $(O)*" "\n" \
"*$(V) parentheses PARSING - Made by jjorda                     $(O)*" "\n" \
"*$(V) Parentheses and parentheses support                      $(O)*" "\n" \
"*$(V) Started : 04/04/2025                                     $(O)*" "\n" \
"**********************************************"