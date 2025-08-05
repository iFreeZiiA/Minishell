# Configuration minimal pour tester executor seulement
MANDATORY_SOURCES = \
	mandatory/main.c \
	mandatory/executor/ast_to_command_list.c \
	mandatory/executor/executor.c \
	mandatory/executor/get_path.c \
	mandatory/builtin/builtin_cd.c \
	mandatory/builtin/builtin_cd_utils.c \
	mandatory/builtin/builtin_echo.c \
	mandatory/builtin/builtin_env.c \
	mandatory/builtin/builtin_exit.c \
	mandatory/builtin/builtin_export.c \
	mandatory/builtin/builtin_pwd.c \
	mandatory/builtin/builtin_unset.c \
	mandatory/builtin/run_builtin.c \
	mandatory/signal/signal.c \
	mandatory/deployment/setup/setup.c \
	mandatory/deployment/cleanup/cleanup.c \
	mandatory/parser/parser.c

BONUS_SOURCES = \
	bonus/main.c

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iheader -Ilib/libft -Ilib/libms
LDFLAGS = -lreadline
