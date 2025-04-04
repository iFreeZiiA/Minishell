clean:
	@make clean -s -C $(DIR_LIB)
	@$(RM)r $(DIR_OBJ)

clean_man:
	@make clean -s -C $(DIR_LIB)
	@$(RM)r $(DIR)/$(MAN)

clean_bns:
	@make clean -s -C $(DIR_LIB)
	@$(RM)r $(DIR_BNS)/$(BNS)
