#                                                    ______                 __
#                                                   / __/ /_     __________/ /
#                                                  / /_/ __/    / ___/ ___/ / 
#                                                 / __/ /_     (__  |__  ) /  
#                                                /_/  \__/____/____/____/_/   
#                                             by: acroue/_____/

################################################################################
#                                 PROGRAM NAME                                 #
################################################################################

NAME = ft_ssl

################################################################################
#                                    COLORS                                    #
################################################################################
DEFAULT    = \033[0m
BLACK    = \033[0;30m
RED        = \033[0;31m
GREEN    = \033[0;32m
YELLOW    = \033[0;33m
BLUE    = \033[0;34m
PURPLE    = \033[0;35m
CYAN    = \033[0;36m
BWHITE    = \033[1;37m

################################################################################
#                               COMPILATION FLAGS                              #
################################################################################
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
DEP_FLAGS = -MMD -MP
RM = rm -rf
LIB_FLAGS = --no-print-directory --silent

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Compilation Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
COMPIL_DEFINES=-DENABLE_MD5=true
COMPIL_DEFINES+=-DENABLE_SHA256=true
LOG_VALUE=P_LOG_DEFAULT
COMPIL_DEFINES+=-DLOG_LEVEL=$(LOG_VALUE)

################################################################################
#                                  DIRECTORIES                                 #
################################################################################
SRCS_DIR = srcs
SSL_DIR = ssl
MD5_DIR = md5
SHA256_DIR = sha256
INCS_DIR = includes
OBJS_DIR = objs

################################################################################
#                                   LIBRARIES                                  #
################################################################################
LIBS_DIR = libs

LIBFT_DIR = $(LIBS_DIR)/libft
LIBFT = $(LIBFT_DIR)/libft.a
PENELOPE_DIR = $(LIBS_DIR)/penelope
PENELOPE = $(PENELOPE_DIR)/penelope.a

LIB :=	$(LIBFT) $(PENELOPE)

################################################################################
#                                    SOURCES                                   #
################################################################################
INCLUDES := $(INCS_DIR) \
			$(LIBFT_DIR) \
			$(PENELOPE_DIR)

INCLUDES_FLAGS := $(addprefix -I , $(INCLUDES))

SRC :=	$(addprefix $(SSL_DIR)/, \
	debug_prints.c \
	errors.c \
	execution.c \
	hash_list.c \
	main.c \
	parsing.c \
	print_digest.c ) \
	$(addprefix $(MD5_DIR)/, \
	main_md5.c \
	md5_algorithm_utils.c \
	md5_algorithm.c \
	md5_blocks_utils.c \
	md5_blocks.c \
	md5_constants.c ) \
	$(addprefix $(SHA256_DIR)/, \
	main_sha256.c \
	sha256_algorithm.c \
	sha256_blocks_utils.c \
	sha256_blocks.c \
	sha256_constants.c \
	sha256_schedule_array.c )

OBJ := $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRC))
DEPS := $(patsubst %.c,$(OBJS_DIR)/%.d,$(SRC))

################################################################################
#                                     RULES                                    #
################################################################################
all: fclean $(NAME)

$(NAME): $(LIBFT) $(PENELOPE) $(OBJ)
	@echo "$(GREEN)* Assembling $(BWHITE)$@$(DEFAULT)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) $(INCLUDES_FLAGS) -o $@

-include %(DEPS)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@echo "$(CYAN)- Compiling$(DEFAULT) $<"
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/$(SSL_DIR)
	@mkdir -p $(OBJS_DIR)/$(MD5_DIR)
	@mkdir -p $(OBJS_DIR)/$(SHA256_DIR)
	@$(CC) $(DEP_FLAGS) $(COMPIL_DEFINES) $(CFLAGS) $(INCLUDES_FLAGS) -c $< -o $@

clean:
	@echo "$(RED)! Removing$(DEFAULT) ${OBJS_DIR} files"
	@${RM} ${OBJS_DIR}

fclean: clean
	@echo "$(RED)! Removing$(DEFAULT) $(NAME)"
	@$(RM) $(NAME)

re: fclean all

cre:
	@clear
	@make re --no-print-directory

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Lib Rules ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
$(LIBFT):
	@echo "$(CYAN)~ Compiling$(DEFAULT) $(PURPLE)$(LIBFT_DIR)$(DEFAULT)"
	@make -C $(LIBFT_DIR) $(LIB_FLAGS)

$(PENELOPE):
	@echo "$(CYAN)~ Compiling$(DEFAULT) $(PURPLE)$(PENELOPE_DIR)$(DEFAULT)"
	@make -C $(PENELOPE_DIR) $(LIB_FLAGS)

cleanlib:
	@echo "$(RED)! Removing$(DEFAULT) $(PURPLE)$(LIBFT_DIR)$(DEFAULT)"
	@make clean -C $(LIBFT_DIR) $(LIB_FLAGS)
	@echo "$(RED)! Removing$(DEFAULT) $(PURPLE)$(PENELOPE_DIR)$(DEFAULT)"
	@make clean -C $(PENELOPE_DIR) $(LIB_FLAGS)

fcleanlib:
	@echo "$(RED)! Removing$(DEFAULT) $(PURPLE)$(LIBFT_DIR)$(DEFAULT)"
	@make fclean -C $(LIBFT_DIR) $(LIB_FLAGS)
	@echo "$(RED)! Removing$(DEFAULT) $(PURPLE)$(PENELOPE_DIR)$(DEFAULT)"
	@make fclean -C $(PENELOPE_DIR) $(LIB_FLAGS)

relib: fcleanlib $(LIBFT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Penelope Rules ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
debug: fclean
	@$(MAKE) LOG_VALUE=P_LOG_DEBUG $(LIB_FLAGS)

trace: fclean
	@$(MAKE) LOG_VALUE=P_LOG_TRACE $(LIB_FLAGS)

info: fclean
	@$(MAKE) LOG_VALUE=P_LOG_INFO $(LIB_FLAGS)
