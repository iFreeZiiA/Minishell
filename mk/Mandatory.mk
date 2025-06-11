all:	$(LIB) $(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BANNER)

dir_mandatory: 
	@mkdir -p $(DIR)$(MAN)
	@mkdir -p $(DIR)$(LEX)
	@mkdir -p $(DIR)$(EXP)
	@mkdir -p $(DIR)$(PRR)
	@mkdir -p $(DIR)$(SUP)
	@mkdir -p $(DIR)$(CUP)

$(DIR)%.o: %.c | dir_mandatory
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@