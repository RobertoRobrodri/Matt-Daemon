# COLORS #
#
GREEN 	= @echo "\033[0;32m"
BLUE 	= @echo "\033[0;34m"
PURPLE 	= @echo "\033[0;35m"
CYAN 	= @echo "\033[0;36m"
RESET 	= "\033[1;0m"

# VARIABLES #
#
NAME 		= matt_daemon_client
CC 			= g++
INCLUDE 	= -std=c++20
CXXFLAGS 	= -Wall -Wextra -g -fsanitize=address -lX11 #-werror

# PATHS #
#
SRC_PATH    	= srcs
SUBFILE1_PATH   = gui
OBJ_PATH    	= objects

# SOURCES #
#
SUBFILE1_SRC = gui.cpp

SRC =	main.cpp \
		$(addprefix $(SUBFILE1_PATH)/, $(SUBFILE1_SRC))
# RULES #
#
all: $(NAME)

SRCS 	  = $(addprefix $(SRC_PATH)/, $(SRC))
OBJS 	  = $(addprefix $(OBJ_PATH)/, $(SRC:%.cpp=%.o))

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE1_PATH))

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp | $(OBJ_PATH)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)
	$(GREEN) Program asembled $(RESET)
	@echo "⠀⠀⠀	    ⣠⣴⣶⣿⣿⣷⣶⣄⣀⣀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣾⣿⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⡟⠁⣰⣿⣿⣿⡿⠿⠻⠿⣿⣿⣿⣿⣧⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⠏⠀⣴⣿⣿⣿⠉⠀⠀⠀⠀⠀⠈⢻⣿⣿⣇⠀⠀⠀\n\
⠀⠀⠀⠀⢀⣠⣼⣿⣿⡏⠀⢠⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⡀⠀⠀\n\
⠀⠀⠀⣰⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠀\n\
⠀⠀⢰⣿⣿⡿⣿⣿⣿⡇⠀⠘⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⢀⣸⣿⣿⣿⠁⠀⠀\n\
⠀⠀⣿⣿⣿⠁⣿⣿⣿⡇⠀⠀⠻⣿⣿⣿⣷⣶⣶⣶⣶⣶⣿⣿⣿⣿⠃⠀⠀⠀\n\
⠀⢰⣿⣿⡇⠀⣿⣿⣿⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀\n\
⠀⢸⣿⣿⡇⠀⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉⢉⣿⣿⠀⠀⠀⠀⠀⠀\n\
⠀⢸⣿⣿⣇⠀⣿⣿⣿⠀⠀⠀⠀⠀⢀⣤⣤⣤⡀⠀⠀⢸⣿⣿⣿⣷⣦⠀⠀⠀\n\
⠀⠀⢻⣿⣿⣶⣿⣿⣿⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣦⡀⠀⠉⠉⠻⣿⣿⡇⠀⠀\n\
⠀⠀⠀⠛⠿⣿⣿⣿⣿⣷⣤⡀⠀⠀⠀⠀⠈⠹⣿⣿⣇⣀⠀⣠⣾⣿⣿⡇⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣦⣤⣤⣤⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣿⣿⣿⣿⣿⣿⠿⠋⠉⠛⠋⠉⠉⠁⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠁\n"

clean:
	$(PURPLE) CLEANING OBJECTS $(RESET)
		rm -rf $(OBJ_PATH)

fclean: clean
	$(PURPLE) CLEANING OBJECTS AND EXEC $(RESET)
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re