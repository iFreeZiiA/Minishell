include mk/configs/Config.mk
include mk/configs/Colors.mk
include mk/configs/Sources.mk
include mk/Mandatory.mk
# include mk/Bonus.mk
include mk/Clean.mk
include mk/Fclean.mk
include mk/Re.mk
include mk/Alearroy.mk
include mk/jjorda.mk
include mk/unit_test/lexing.mk
include mk/unit_test/parsing.mk
include mk/unit_test/setup.mk
include mk/unit_test/cleanup.mk

$(LIB) :
	@make -s -C $(DIR_LIBMS);
	@make -s -C $(DIR_LIBFT);

dir_utils :
	@mkdir -p $(UTL)