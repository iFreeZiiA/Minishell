NAME_PR	= parser_ut

DIR_PR	= $(DIR_UT)/
PRR		= $(MAN)/parser/parsing/

SRC_PRR	= $(PRR)parsing.c $(PRR)ast_utils.c $(PRR)parse_redirection.c
UT_PRR	= $(PRR)main_test.c $(SRC_PRR) $(SRC_LXR)

OBJ_PRR	= $(patsubst %.c, $(DIR_PR)%.o, $(UT_PRR))

parsing:	$(LIB) $(NAME_PR)

$(NAME_PR):	$(OBJ_PRR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PRR) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BAN_PR)

dir_parser: dir_lexer
	@mkdir -p $(DIR_PR)$(PRR)

$(DIR_PR)$(PRR)%.o: $(PRR)%.c | dir_parser
	@$(CC) $(CFLAGS) -c $< -o $@

BAN_PR	= \
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
