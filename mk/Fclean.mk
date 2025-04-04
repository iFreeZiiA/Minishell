fclean:		clean
	@make fclean -s -C $(DIR_LIB)
	@$(RM) $(NAME) $(NAME_BNS)

fclean_man:	clean
	@make fclean -s -C $(DIR_LIB)
	@$(RM) $(NAME)

fclean_bns:	clean
	@make fclean -s -C $(DIR_LIB)
	@$(RM) $(NAME_BNS)