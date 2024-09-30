# COLORS #
#
GREEN 	= @echo "\033[0;32m"
BLUE 	= @echo "\033[0;34m"
PURPLE 	= @echo "\033[0;35m"
CYAN 	= @echo "\033[0;36m"
RESET 	= "\033[1;0m"

# VARIABLES #
#
NAME 		= matt_daemon
CC 			= g++
INCLUDE 	= -std=c++20
CXXFLAGS 	= -Wall -Wextra -g -fsanitize=address -MMD -MP#-werror

# PATHS #
#
SRC_PATH    	= srcs
SUBFILE1_PATH   = reporter
SUBFILE2_PATH   = daemon
OBJ_PATH    	= objects

# SOURCES #
#
SUBFILE1_SRC = reporter.cpp
SUBFILE2_SRC = daemon.cpp


SRC =	main.cpp \
		$(addprefix $(SUBFILE1_PATH)/, $(SUBFILE1_SRC)) \
		$(addprefix $(SUBFILE2_PATH)/, $(SUBFILE2_SRC))

# RULES #
#
all: $(NAME)

SRCS 	  = $(addprefix $(SRC_PATH)/, $(SRC))
OBJS 	  = $(addprefix $(OBJ_PATH)/, $(SRC:%.cpp=%.o))

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE1_PATH))
	mkdir -p $(addprefix $(OBJ_PATH)/, $(SUBFILE2_PATH))

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp | $(OBJ_PATH)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS) Makefile
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

client:
	$(MAKE) -C ./client/

bonus: all client

-include $(OBJS:.o=.d)
clean:
	$(PURPLE) CLEANING OBJECTS $(RESET)
		rm -rf $(OBJ_PATH)

fclean: clean
	$(PURPLE) CLEANING OBJECTS AND EXEC $(RESET)
		rm -rf $(NAME)
	$(MAKE) fclean -C ./client/
re: fclean all

.PHONY: all clean fclean re client
