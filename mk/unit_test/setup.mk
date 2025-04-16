NAME_SUP= setup_ut

DIR_SUP	= $(DIR_OBJ)/
SUP		= $(MAN)/setup/

SRC_SUP	= $(SUP)setup.c 
UT_SUP	= $(SUP)main_test.c $(SRC_SUP)

OBJ_SUP	= $(patsubst %.c, $(DIR_SUP)%.o, $(UT_SUP))

setup:	$(LIB) $(NAME_SUP)

$(NAME_SUP):	$(OBJ_SUP)
	$(CC) $(CFLAGS) -o $@ $(OBJ_SUP) -L$(DIR_LIB) -lft
	@$(PRINT) $(BANNER)

dir_setup:
	@mkdir -p $(DIR_SUP)$(SUP)

# $(DIR_SUP)$(SUP)%.o: $(SUP)%.c | dir_setup
# 	@$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SUP)$(SUP).main.o: $(SUP).main.c | dir_setup
	@$(CC) $(CFLAGS) -c $< -o $@