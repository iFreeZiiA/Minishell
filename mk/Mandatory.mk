all:	$(LIB) $(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_mandatory:
	@mkdir -p $(DIR)/$(MAN)
	@mkdir -p $(DIR_BNS)/$(BNS)

$(DIR)%.o: %.c | dir_mandatory
	@$(CC) $(CFLAGS) -c $< -o $@