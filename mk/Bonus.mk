bonus:	$(LIB) $(NAME_BNS)

$(NAME_BNS): $(OBJ_BNS)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_BNS) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER_BNS)

dir_bonus:
	@mkdir -p $(DIR_BNS)/$(BNS)

$(DIR_BNS)%.o: %.c | dir_bonus
	@$(CC) $(CFLAGS) -c $< -o $@