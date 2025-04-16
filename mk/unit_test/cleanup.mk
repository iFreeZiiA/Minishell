NAME_CUP= cleanup_ut

DIR_CUP	= $(DIR_OBJ)/
CUP		= $(MAN)/cleanup/

SRC_CUP	= $(CUP)cleanup.c 
UT_CUP	= $(CUP).main.c $(SRC_CUP)

OBJ_CUP	= $(patsubst %.c, $(DIR_CUP)%.o, $(UT_CUP))

cleanup:	$(LIB) $(NAME_CUP)

$(NAME_CUP):	$(OBJ_CUP)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_CUP) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_cleanup:
	@mkdir -p $(DIR_CUP)$(CUP)

$(DIR_CUP)$(CUP)%.o: $(CUP)%.c | dir_cleanup
	@$(CC) $(CFLAGS) -c $< -o $@