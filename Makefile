NAME = ircserv

CC = c++
FLAGS = -Wall -Wextra -Werror

SOURCE_DIR		= sources
SOURCE_CMD_DIR	= $(SOURCE_DIR)/commands
INCLUDE_DIR		= includes
INCLUDE_CMD_DIR	= $(INCLUDE_DIR)/commands
OBJDIR			= obj

HEADERS		= Client.hpp Server.hpp Channel.hpp CmdList.hpp \
			errors.hpp macros.hpp
CMD_HEADERS	= ACommand.hpp PassCmd.hpp UserCmd.hpp NickCmd.hpp \
			PingCmd.hpp QuitCmd.hpp JoinCmd.hpp KickCmd.hpp PartCmd.hpp \
			PrivMsgCmd.hpp InviteCmd.hpp ModeCmd.hpp TopicCmd.hpp
ALL_HEADERS	= $(addprefix $(INCLUDE_DIR)/, $(HEADERS)) \
			$(addprefix $(INCLUDE_CMD_DIR)/, $(CMD_HEADERS))

SOURCES		= main.cpp inputValidation.cpp \
			Client.cpp Server.cpp Channel.cpp CmdList.cpp
CMD_SOURCES	= ACommand.cpp PassCmd.cpp UserCmd.cpp NickCmd.cpp \
			PingCmd.cpp QuitCmd.cpp JoinCmd.cpp KickCmd.cpp PartCmd.cpp \
			PrivMsgCmd.cpp InviteCmd.cpp ModeCmd.cpp TopicCmd.cpp

OBJECTS		= $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
CMD_OBJECTS	= $(addprefix $(OBJDIR)/commands/, $(CMD_SOURCES:.cpp=.o))
ALL_OBJECTS	= $(OBJECTS) $(CMD_OBJECTS)
INCLUDES	= -I$(INCLUDE_DIR) -I$(INCLUDE_CMD_DIR)

all: $(NAME)

$(NAME): $(ALL_OBJECTS)
	$(CC) $(FLAGS) -o $(NAME) $(ALL_OBJECTS)

$(OBJDIR)/%.o: $(SOURCE_DIR)/%.cpp  $(ALL_HEADERS) | $(OBJDIR)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@
$(OBJDIR)/commands/%.o: $(SOURCE_CMD_DIR)/%.cpp $(ALL_HEADERS) | $(OBJDIR)/commands
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR) $(OBJDIR)/commands:
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re