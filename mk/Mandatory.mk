all:	$(LIB) $(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BANNER)

dir_mandatory: 
	@mkdir -p $(DIR)/$(MAN)
	@mkdir -p $(DIR)/$(LGOP)
	@mkdir -p $(DIR)/$(PRR)
	@mkdir -p $(DIR)/$(SBS)
	@mkdir -p $(DIR)/$(LEX)
	@mkdir -p $(DIR)/$(CUP)
	@mkdir -p $(DIR)/$(SUP)

$(DIR)%.o: %.c | dir_mandatory
	@$(CC) $(CFLAGS) -c $< -o $@