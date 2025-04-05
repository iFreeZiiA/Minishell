DIR_J	= $(DIR_OBJ)/
LEX		= $(MAN)lexing/
SRC_J	= $(LEX)main.c $(LEX)lexing.c
OBJ_J	= $(patsubst %.c, $(DIR_J)%.o, $(SRC_J))

jj:	$(LIB) jjorda

jjorda:	$(OBJ_J)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_J) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_jjorda:
	@mkdir -p $(DIR_J)$(LEX)

$(DIR_J)%.o: %.c | dir_jjorda
	@$(CC) $(CFLAGS) -c $< -o $@