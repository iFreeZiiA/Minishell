NAME_CUP= cleanup_ut

DIR_CUP	= $(DIR_OBJ)/
CUP		= $(MAN)/deployment/cleanup/

SRC_CUP	= $(CUP)cleanup.c $(SRC_SUP)
UT_CUP	= $(CUP)main_test.c $(SRC_CUP)

OBJ_CUP	= $(patsubst %.c, $(DIR_CUP)%.o, $(UT_CUP))

cleanup:	$(LIB) $(NAME_CUP)

$(NAME_CUP):	$(OBJ_CUP)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_CUP) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_cleanup:
	@mkdir -p $(DIR_CUP)$(CUP)

$(DIR_CUP)$(CUP)%.o: $(CUP)%.c | dir_cleanup
	@$(CC) $(CFLAGS) -c $< -o $@