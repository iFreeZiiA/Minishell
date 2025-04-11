NAME_PR	= parser

DIR_PR	= $(DIR_OBJ)/
PRR		= $(MAN)/parsing/

SRC_PRR	= $(PRR)parsing.c
UT_PRR	= $(PRR).main.c $(SRC_PRR) $(SRC_LXR)

OBJ_PRR	= $(patsubst %.c, $(DIR_PR)%.o, $(SRC_PRR))

parsing:	$(LIB) $(NAME_PR)

$(NAME_PR):	$(OBJ_PRR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_PRR) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_parser: dir_lexer
	@mkdir -p $(DIR_PR)$(PRR)

$(DIR_PR)$(PRR)%.o: $(PRR)%.c | dir_parser | dir_lexer
	@$(CC) $(CFLAGS) -c $< -o $@