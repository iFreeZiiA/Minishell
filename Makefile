include mk/configs/Config.mk
include mk/configs/Colors.mk
include mk/configs/Sources.mk
include mk/Mandatory.mk
# include mk/Bonus.mk
include mk/Clean.mk
include mk/Fclean.mk
include mk/Re.mk

$(LIB) :
	@make -s -C $(DIR_LIB);