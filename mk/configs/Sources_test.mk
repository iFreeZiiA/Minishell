# Sources minimales pour tester nos corrections
SRC_MAIN = $(MAN)main.c

SRC_BUILTIN = $(MAN)builtin/builtin_cd_utils.c $(MAN)builtin/builtin_pwd.c \
	$(MAN)builtin/run_builtin.c $(MAN)builtin/builtin_unset.c \
	$(MAN)builtin/builtin_export.c $(MAN)builtin/builtin_env.c \
	$(MAN)builtin/builtin_exit.c $(MAN)builtin/builtin_echo.c \
	$(MAN)builtin/builtin_cd.c

SRC_SIGNAL = $(MAN)signal/signal.c

SRC_EXECUTOR = $(MAN)executor/executor.c $(MAN)executor/ast_to_command_list.c \
	$(MAN)executor/get_path.c

SRC_DEPLOYMENT = $(MAN)deployment/setup/setup.c $(MAN)deployment/setup/environment.c \
	$(MAN)deployment/cleanup/cleanup.c

# Parser - fichiers principaux seulement (sans les problématiques)
SRC_PARSER_BASE = $(MAN)parser/enhanced.c $(MAN)parser/parser.c

SRC_LEXING = $(MAN)parser/lexing/err.c $(MAN)parser/lexing/utils.c \
	$(MAN)parser/lexing/extract.c $(MAN)parser/lexing/tokenize.c \
	$(MAN)parser/lexing/variables.c $(MAN)parser/lexing/parse_var.c \
	$(MAN)parser/lexing/expand.c

# Parsing - seulement les fichiers qui compilent
SRC_PARSING_CORE = $(MAN)parser/parsing/ast/create.c \
	$(MAN)parser/parsing/ast/free.c \
	$(MAN)parser/parsing/logical/logical.c \
	$(MAN)parser/parsing/logical/precedence.c \
	$(MAN)parser/parsing/parentheses/check.c \
	$(MAN)parser/parsing/parentheses/find.c \
	$(MAN)parser/parsing/parentheses/parentheses.c \
	$(MAN)parser/parsing/pipes/operators.c \
	$(MAN)parser/parsing/pipes/pipes.c \
	$(MAN)parser/parsing/redir/input.c \
	$(MAN)parser/parsing/redir/output.c \
	$(MAN)parser/parsing/redir/redir.c \
	$(MAN)parser/parsing/redir/validate.c \
	$(MAN)parser/parsing/utils/ast.c \
	$(MAN)parser/parsing/utils/logical.c \
	$(MAN)parser/parsing/utils/parser.c \
	$(MAN)parser/parsing/utils/redir.c \
	$(MAN)parser/parsing/utils/token.c \
	$(MAN)parser/parsing/parse.c \
	$(MAN)parser/parsing/parser.c \
	$(MAN)parser/parsing/parser_util.c \
	$(MAN)parser/parsing/utils.c \
	$(MAN)parser/parsing/heredoc/heredoc.c \
	$(MAN)parser/parsing/logic_operator/parsing_utils.c \
	$(MAN)parser/parsing/logic_operator/print_utils.c

SRC_UTILS = $(MAN)utils/missing_functions.c

# Sources minimales pour test
SRC = $(SRC_MAIN) $(SRC_BUILTIN) $(SRC_SIGNAL) $(SRC_EXECUTOR) \
	$(SRC_DEPLOYMENT) $(SRC_PARSER_BASE) $(SRC_LEXING) $(SRC_PARSING_CORE) $(SRC_UTILS)
