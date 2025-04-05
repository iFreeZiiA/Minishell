NAME_A	= alearroy
DIR_A	= obj/

BLT	= builtin/

SRC_A	= $(BLT)builtin_echo.c $(BLT)builtin_pwd.c $(BLT)run_builtin.c

OBJ_A	= $(patsubst %.c, $(DIR_A)%.o, $(SRC_A))

alearroy:	$(LIB) $(NAME_A)

$(NAME_A):	$(OBJ_A)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_A) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_mandatory_a:
	@mkdir -p $(DIR_A)/$(BLT)

$(DIR_A)%.o: %.c | dir_mandatory_a
	@$(CC) $(CFLAGS) -c $< -o $@