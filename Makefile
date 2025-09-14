NAME = n-puzzle

CPP = g++ -std=c++20

CFLAGS = -Wall -Werror -Wextra -Wno-shadow -Wconversion

INC = -I ./include/.

SRC_DIR = ./srcs/
	  
SRC_FILES = main.cpp print.cpp input.cpp heuristics.cpp

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR = .objs/

OBJ = $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

PY = python

GEN = case-gen.py

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	if [ ! -f $(NAME) ] || [ `find $(OBJ) -newer $(NAME) | wc -l` -ne 0 ]; then \
	$(CPP) $(CFLAGS) $(INC) $(OBJ) -o $(NAME); \
		printf "$(ERASE)$(GREEN)👷 Program$(RESET) $(CYAN)$(BOLD)$(NAME)$(RESET) $(GREEN)created! 👷\n$(RESET)"; \
	else \
		printf "$(ERASE)$(YELLOW)No relink needed for$(RESET) $(CYAN)$(BOLD)$(NAME)\n$(RESET)"; \
	fi \

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	${CPP} $(CFLAGS) -c $< -o $@
	printf "$(ERASE)$(BLUE) > Compilation: $(RESET) $<"

gen:
	$(PY) $(GEN)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	printf "$(ERASE)$(GREEN)Fcleaning up...$(RESET)"
	$(RM) ${NAME}
	printf "$(ERASE)🧼 $(GREEN)Fclean finished! 🧼\n$(RESET)"

re: fclean all

.PHONY: all clean fclean re 
.SILENT:

# COLORS
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
BOLD = \033[1m
ERASE = \033[2K\r
RESET = \033[0m
