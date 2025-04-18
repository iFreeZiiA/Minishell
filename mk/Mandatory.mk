all:	$(LIB) $(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS)
	@$(PRINT) $(BANNER)

dir_mandatory:
	@mkdir -p $(DIR)/$(MAN)

# $(DIR)%.o: %.c | dir_mandatory
# 	@$(CC) $(CFLAGS) -c $< -o $@