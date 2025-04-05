fclean:		clean
	@make fclean -s -C $(DIR_LIB)
	@$(RM) $(NAME) $(NAME_BNS)

fclean_man:	clean_man
	@make fclean -s -C $(DIR_LIB)
	@$(RM) $(NAME)

fclean_bns:	clean_bns
	@make fclean -s -C $(DIR_LIB)
	@$(RM) $(NAME_BNS)