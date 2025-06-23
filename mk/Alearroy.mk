NAME_A	= alearroy
DIR_A	= obj

MAN		= mandatory/
BLT		= $(MAN)builtin/
SGL		= $(MAN)signal/
EXE		= $(MAN)executor/

SRC_A	= $(BLT)builtin_echo.c $(BLT)builtin_pwd.c					\
		$(BLT)run_builtin.c $(BLT)builtin_cd.c $(BLT)builtin_env.c	\
		$(BLT)builtin_exit.c $(BLT)builtin_export.c					\
		$(BLT)builtin_unset.c $(SGL)signal.c $(MAN)main.c			\
		$(BLT)builtin_cd_utils.c $(EXE)ast_to_command_list.c		\
		$(EXE)executor.c $(EXE)get_path.c $(EXE)pipe.c				\
		$(EXE)pipe_utils.c $(EXE)free_command_list.c				\
		$(EXE)heredoc.c												\

OBJ_A	= $(patsubst %.c, $(DIR_A)/%.o, $(SRC_A))

# LIBS	= -lft -lreadline

alearroy: $(LIB) $(NAME_A)

$(NAME_A): $(OBJ_A)
	@$(CC) $(CFLAGS) -o $@ $(OBJ_A) -L$(DIR_LIB) $(LIBS)
	@$(PRINT) $(BANNER)

dir_mandatory_a:
	@mkdir -p $(DIR_A)/$(MAN)
	@mkdir -p $(DIR_A)/$(BLT)
	@mkdir -p $(DIR_A)/$(SGL)
	@mkdir -p $(DIR_A)/$(EXE)

# $(DIR_A)/%.o: %.c | dir_mandatory_a
# 	@$(CC) $(CFLAGS) -c $< -o $@