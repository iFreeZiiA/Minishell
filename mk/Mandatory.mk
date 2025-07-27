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
	@mkdir -p $(DIR)/$(MAN)parser/lexing
	@mkdir -p $(DIR)/$(MAN)parser/lexing/expansion
	@mkdir -p $(DIR)/$(MAN)parser/lexing/wildcard
	@mkdir -p $(DIR)/$(MAN)parser/parsing
	@mkdir -p $(DIR)/$(MAN)parser/parsing/parentheses
	@mkdir -p $(DIR)/$(MAN)parser/parsing/pipes
	@mkdir -p $(DIR)/$(MAN)parser/parsing/logical
	@mkdir -p $(DIR)/$(MAN)parser/parsing/redir
	@mkdir -p $(DIR)/$(MAN)parser/parsing/heredoc
	@mkdir -p $(DIR)/$(MAN)parser/parsing/validate
	@mkdir -p $(DIR)/$(MAN)parser/parsing/utils
	@mkdir -p $(DIR)/$(MAN)utils

# Règles de compilation pour tous les fichiers .c dans les sous-répertoires
$(DIR)%.o: %.c | dir_mandatory
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"
