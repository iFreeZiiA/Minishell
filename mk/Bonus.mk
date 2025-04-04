bonus:	$(LIB) $(NAME_BNS)

$(NAME_BNS): $(OBJ_BNS)
	@$(CC) $(CFLAGS) $(OBJ_BNS) -o $@ -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER_BNS)

dir_bonus:
	@mkdir -p $(DIR_BNS)

$(DIR_BNS)%.o: %.c | dir_bonus
	@$(CC) $(CFLAGS) -c $< -o $@