NAME_LX	= lexer

DIR_LX	= $(DIR_UT)
LEX		= $(MAN)/lexing/

SRC_LXR	= $(LEX)lexing.c $(LEX)err.c $(LEX)utils.c
UT_LXR	= $(LEX).main.c $(SRC_LXR)

OBJ_LXR	= $(patsubst %.c, $(DIR_LX)%.o, $(UT_LXR))

lexing:	$(LIB) $(NAME_LX)

$(NAME_LX):	$(OBJ_LXR)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_LXR) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_lexer:
	@mkdir -p $(DIR_LX)$(LEX)

$(DIR_LX)$(LEX)%.o: $(LEX)%.c | dir_lexer
	@$(CC) $(CFLAGS) -c $< -o $@
