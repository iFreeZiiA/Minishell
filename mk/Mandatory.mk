all:	$(LIB) $(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBFT) $(LIBMS) $(LIBFT) -lreadline
	@$(PRINT) $(BANNER)

dir_mandatory: 
	@mkdir -p $(DIR)/$(MAN)
	@mkdir -p $(DIR)/$(MAN)builtin
	@mkdir -p $(DIR)/$(MAN)signal
	@mkdir -p $(DIR)/$(MAN)deployment/setup
	@mkdir -p $(DIR)/$(MAN)deployment/cleanup
	@mkdir -p $(DIR)/$(MAN)executor
	@mkdir -p $(DIR)/$(MAN)parser
	@mkdir -p $(DIR)/$(MAN)parser/src/lexing
	@mkdir -p $(DIR)/$(MAN)parser/src/parsing/groups
	@mkdir -p $(DIR)/$(MAN)parser/src/parsing/pipe
	@mkdir -p $(DIR)/$(MAN)parser/src/parsing/logic
	@mkdir -p $(DIR)/$(MAN)parser/src/parsing/redirect
	@mkdir -p $(DIR)/$(MAN)parser/src/parsing/heredoc
	@mkdir -p $(DIR)/$(MAN)parser/src/parsing/utils
	@mkdir -p $(DIR)/$(MAN)utils

# Règles de compilation pour tous les fichiers .c dans les sous-répertoires
$(DIR)%.o: %.c | dir_mandatory
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"
