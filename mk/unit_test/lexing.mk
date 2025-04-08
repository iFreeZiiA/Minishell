NAME_LX	= lexer
DIR_LX	= $(DIR_OBJ)/
LEX		= $(MAN)lexing/
SRC_LXR	= $(LEX).main.c $(LEX)lexing.c $(LEX)err.c $(LEX)utils.c
OBJ_LXR	= $(patsubst %.c, $(DIR_LX)%.o, $(SRC_LXR))

lexing:	$(LIB) $(NAME_LX)

$(NAME_LX):	$(OBJ_LXR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_LXR) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_lexer:
	@mkdir -p $(DIR_LX)$(LEX)

$(DIR_LX)%.o: %.c | dir_lexer
	@$(CC) $(CFLAGS) -c $< -o $@