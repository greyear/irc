NAME = ircserv

CC = c++
FLAGS = -Wall -Wextra -Werror

OBJDIR = obj

HEADERS = Client.hpp Server.hpp Channel.hpp ACommand.hpp PassCmd.hpp UserCmd.hpp NickCmd.hpp \
		PingCmd.hpp PrivMsgCmd.hpp JoinCmd.hpp PartCmd.hpp \
		CmdList.hpp errors.hpp macros.hpp QuitCmd.hpp InviteCmd.hpp
SOURCES = main.cpp inputValidation.cpp Client.cpp Server.cpp Channel.cpp \
		ACommand.cpp PassCmd.cpp UserCmd.cpp NickCmd.cpp \
		PingCmd.cpp CmdList.cpp PrivMsgCmd.cpp JoinCmd.cpp \
		PartCmd.cpp QuitCmd.cpp InviteCmd.cpp
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJECTS)

$(OBJDIR)/%.o: %.cpp  $(HEADERS) | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re