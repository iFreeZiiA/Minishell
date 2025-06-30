all:	$(LIB) $(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BANNER)

dir_mandatory: 
	@mkdir -p $(DIR)/$(MAN)
	@mkdir -p $(DIR)/$(LEX)
	@mkdir -p $(DIR)/$(PRT)
# @mkdir -p $(DIR)/$(PRR)
	@mkdir -p $(DIR)/$(EXP_DIR)
	@mkdir -p $(DIR)/$(AST_DIR)
	@mkdir -p $(DIR)/$(SUP)
	@mkdir -p $(DIR)/$(CUP)
	@mkdir -p $(DIR)/$(EXP)
	@mkdir -p $(DIR)/$(BLT)
	@mkdir -p $(DIR)/$(SGL)
	@mkdir -p $(DIR)/$(EXE)

$(DIR)%.o: %.c | dir_mandatory
	@$(CC) $(CFLAGS) -c $< -o $@
