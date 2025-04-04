include mk/configs/Config.mk
include mk/configs/Colors.mk
include mk/configs/Sources.mk
include mk/Mandatory.mk
include mk/Bonus.mk
include mk/Clean.mk
include mk/Fclean.mk
include mk/Re.mk

$(LIB) :
	@make -s -C $(DIR_LIB);

# bonus:	$(LIB) $(NAME_BNS)

# $(NAME_BNS): dir_bonus $(OBJ_BNS)
# 	@$(CC) $(CFLAGS) $(OBJ_BNS) -o $@ -L$(DIR_LIB) -lft
# 	@$(PRINT) $(BANNER_BNS)

# dir_bonus:
# 	@mkdir -p $(DIR_BNS)

# $(DIR_BNS)%.o: %.c
# 	@$(CC) $(CFLAGS) -c $< -o $@