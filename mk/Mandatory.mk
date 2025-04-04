all:	$(LIB) $(NAME)

$(NAME):	dir_mandatory $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_mandatory:
	@mkdir -p $(MAN)

$(DIR)%.o: %.c