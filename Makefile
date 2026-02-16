NAME		= ircserv
TEST_NAME	= ircserv_tests
COVERAGE_DIR	= coverage

CC			= c++
FLAGS		= -Wall -Wextra -Werror
TEST_FLAGS	= --coverage -fprofile-arcs -ftest-coverage

SOURCE_DIR		= sources
SOURCE_CMD_DIR	= $(SOURCE_DIR)/commands
INCLUDE_DIR		= includes
INCLUDE_CMD_DIR	= $(INCLUDE_DIR)/commands
OBJDIR			= obj
TEST_DIR		= tests

HEADERS			= Client.hpp Server.hpp Channel.hpp CmdList.hpp \
				errors.hpp macros.hpp
CMD_HEADERS		= ACommand.hpp PassCmd.hpp UserCmd.hpp NickCmd.hpp \
				PingCmd.hpp QuitCmd.hpp JoinCmd.hpp KickCmd.hpp PartCmd.hpp \
				PrivMsgCmd.hpp InviteCmd.hpp ModeCmd.hpp TopicCmd.hpp
ALL_HEADERS		= $(addprefix $(INCLUDE_DIR)/, $(HEADERS)) \
				$(addprefix $(INCLUDE_CMD_DIR)/, $(CMD_HEADERS))

SOURCES			= main.cpp inputValidation.cpp \
				Client.cpp Server.cpp Channel.cpp CmdList.cpp
CMD_SOURCES		= ACommand.cpp PassCmd.cpp UserCmd.cpp NickCmd.cpp \
				PingCmd.cpp QuitCmd.cpp JoinCmd.cpp KickCmd.cpp PartCmd.cpp \
				PrivMsgCmd.cpp InviteCmd.cpp ModeCmd.cpp TopicCmd.cpp

TEST_SOURCES	= $(TEST_DIR)/test_main.cpp \
				$(TEST_DIR)/test_validation.cpp \
				$(TEST_DIR)/test_channel.cpp \
				$(TEST_DIR)/test_client.cpp \
				$(TEST_DIR)/test_protocol.cpp \
				$(SOURCE_DIR)/inputValidation.cpp \
				$(SOURCE_DIR)/Channel.cpp \
				$(SOURCE_DIR)/Client.cpp

OBJECTS			= $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
CMD_OBJECTS		= $(addprefix $(OBJDIR)/commands/, $(CMD_SOURCES:.cpp=.o))
ALL_OBJECTS		= $(OBJECTS) $(CMD_OBJECTS)
INCLUDES		= -I$(INCLUDE_DIR) -I$(INCLUDE_CMD_DIR)


all: $(NAME)

test: $(TEST_NAME)

$(NAME): $(ALL_OBJECTS)
	$(CC) $(FLAGS) -o $(NAME) $(ALL_OBJECTS)

$(TEST_NAME): $(TEST_SOURCES)
	$(CC) $(FLAGS) $(TEST_FLAGS) $(INCLUDES) $(TEST_SOURCES) -o $(TEST_NAME)

$(OBJDIR)/%.o: $(SOURCE_DIR)/%.cpp  $(ALL_HEADERS) | $(OBJDIR)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@
$(OBJDIR)/commands/%.o: $(SOURCE_CMD_DIR)/%.cpp $(ALL_HEADERS) | $(OBJDIR)/commands
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR) $(OBJDIR)/commands:
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

coverage: test
	@echo "Running tests to generate coverage data..."
	@./$(TEST_NAME)
	@mkdir -p $(COVERAGE_DIR)
	@lcov --directory . --capture --output-file $(COVERAGE_DIR)/coverage.info --exclude '/usr/*'
	@genhtml $(COVERAGE_DIR)/coverage.info --output-directory $(COVERAGE_DIR)/html
	@echo "Coverage report generated in $(COVERAGE_DIR)/html/index.html"

fclean: clean
	rm -f $(NAME) $(TEST_NAME)
	rm -rf $(COVERAGE_DIR) *.gcda *.gcno

re: fclean all

.PHONY: all clean fclean re test coverage