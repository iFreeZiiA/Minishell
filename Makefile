include mk/configs/Config.mk
include mk/configs/Colors.mk
include mk/configs/Sources.mk
include mk/Mandatory.mk
include mk/Bonus.mk

$(LIB) :
	@make -s -C $(DIR_LIB);
