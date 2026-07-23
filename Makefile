#                                                    ______                 __
#                                                   / __/ /_     __________/ /
#                                                  / /_/ __/    / ___/ ___/ / 
#                                                 / __/ /_     (__  |__  ) /  
#                                                /_/  \__/____/____/____/_/   
#                                             by: acroue/_____/

################################################################################
#                                 PROGRAM NAME                                 #
################################################################################

NAME = arguments

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


################################################################################
#                                  DIRECTORIES                                 #
################################################################################
SRCS_DIR = srcs
MAIN_DIR = main
ARGLIB_DIR = arglib
INCS_DIR = includes
OBJS_DIR = objs

################################################################################
#                                   LIBRARIES                                  #
################################################################################
# LIBS_DIR = libs

# LIBFT_DIR = $(LIBS_DIR)/libft
# LIBFT = $(LIBFT_DIR)/libft.a
# PENELOPE_DIR = $(LIBS_DIR)/penelope
# PENELOPE = $(PENELOPE_DIR)/penelope.a

# LIB :=	$(LIBFT) $(PENELOPE)

################################################################################
#                                    SOURCES                                   #
################################################################################
INCLUDES := $(SRCS_DIR)/$(MAIN_DIR) \
			$(SRCS_DIR)/$(ARGLIB_DIR)

INCLUDES_FLAGS := $(addprefix -I , $(INCLUDES))

SRC :=	$(addprefix $(MAIN_DIR)/, \
	main.c ) \
	$(addprefix $(ARGLIB_DIR)/, \
	arglib.c )

OBJ := $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRC))
DEPS := $(patsubst %.c,$(OBJS_DIR)/%.d,$(SRC))

################################################################################
#                                     RULES                                    #
################################################################################
all: fclean $(NAME)

$(NAME): $(LIBFT) $(PENELOPE) $(OBJ)
	@printf "$(GREEN)* Assembling $(BWHITE)%s$(DEFAULT)\n" "$@"
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) $(INCLUDES_FLAGS) -o $@

-include %(DEPS)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@printf "$(CYAN)- Compiling$(DEFAULT) %s\n" "$<"
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/$(MAIN_DIR)
	@mkdir -p $(OBJS_DIR)/$(ARGLIB_DIR)
	@$(CC) $(DEP_FLAGS) $(COMPIL_DEFINES) $(CFLAGS) $(INCLUDES_FLAGS) -c $< -o $@

clean:
	@printf "$(RED)! Removing$(DEFAULT) ${OBJS_DIR} files\n"
	@${RM} ${OBJS_DIR}

fclean: clean
	@printf "$(RED)! Removing$(DEFAULT) %s\n" "$(NAME)"
	@$(RM) $(NAME)

re: fclean all

cre:
	@clear
	@make re --no-print-directory

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Lib Rules ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
$(LIBFT):
	@printf "$(CYAN)~ Compiling$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(LIBFT_DIR)"
	@make -C $(LIBFT_DIR) $(LIB_FLAGS)

$(PENELOPE):
	@printf "$(CYAN)~ Compiling$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(PENELOPE_DIR)"
	@make -C $(PENELOPE_DIR) $(LIB_FLAGS)

cleanlib:
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(LIBFT_DIR)"
	@make clean -C $(LIBFT_DIR) $(LIB_FLAGS)
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(PENELOPE_DIR)"
	@make clean -C $(PENELOPE_DIR) $(LIB_FLAGS)

fcleanlib:
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(LIBFT_DIR)"
	@make fclean -C $(LIBFT_DIR) $(LIB_FLAGS)
	@printf "$(RED)! Removing$(DEFAULT) $(PURPLE)%s$(DEFAULT)\n" "$(PENELOPE_DIR)"
	@make fclean -C $(PENELOPE_DIR) $(LIB_FLAGS)

relib: fcleanlib $(LIBFT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Penelope Rules ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
debug: fclean
	@$(MAKE) LOG_VALUE=P_LOG_DEBUG $(LIB_FLAGS)

trace: fclean
	@$(MAKE) LOG_VALUE=P_LOG_TRACE $(LIB_FLAGS)

info: fclean
	@$(MAKE) LOG_VALUE=P_LOG_INFO $(LIB_FLAGS)
