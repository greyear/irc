#include <cassert>
#include <string>
#include <iostream>
#include "../includes/Client.hpp"
#include "../includes/macros.hpp"

// Test basic client initialization
void test_client_basic()
{
	Client client(42);

	// Check default values
	assert(client.getFd() == 42);
	assert(client.getNick() == "");
	assert(client.getUser() == "");
	
	// Set and check values
	client.setNick("alice");
	client.setUser("user123");
	client.setRealName("Alice Smith");
	client.setHostName("localhost");
	
	assert(client.getNick() == "alice");
	assert(client.getUser() == "user123");
	assert(client.getRealName() == "Alice Smith");
	assert(client.getHostName() == "localhost");
	
	// Check full identifier construction
	std::string fullId = client.getFullIdentifier();
	assert(fullId == "alice!user123@localhost");

	std::cout << "  ✓ client_basic tests passed" << std::endl;
}

// Test client registration (with commands PASS, USER, NICK)
void test_client_registration()
{
	Client client(1);
	
	// Initially not registered
	assert(client.getHasPass() == false);
	assert(client.getHasUser() == false);
	assert(client.getHasNick() == false);
	assert(client.checkRegistrationComplete() == false);
	
	// Set registration flags in different orders and check completion status
	client.setHasPass(true);
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasUser(true);
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasNick(true);
	assert(client.checkRegistrationComplete() == true);

	std::cout << "  ✓ client_registration tests passed" << std::endl;
}

// Test read buffer message extraction
void test_client_buffers()
{
	Client client(1);
	
	// No complete message yet
	assert(client.hasCompleteMessage() == false);
	
	// Add partial message and check that it is not complete
	client.appendToReadBuffer("NICK alice", 10);
	assert(client.hasCompleteMessage() == false);
	
	// Complete the message and check extraction
	client.appendToReadBuffer("\r\n", 2);
	assert(client.hasCompleteMessage() == true);
	
	// Extract message and check content
	std::string msg = client.extractNextMessage();
	assert(msg == "NICK alice");
	assert(client.hasCompleteMessage() == false);

	std::cout << "  ✓ client_buffers tests passed" << std::endl;
}

// Test extraction of multiple messages from buffer
void test_client_multiple_messages()
{
	Client client(1);
	
	// Add multiple messages at once
	client.appendToReadBuffer("NICK alice\r\nUSER alice 0 * :Alice\r\nPING", 38);
	
	// Extract first message
	assert(client.hasCompleteMessage() == true);
	std::string msg1 = client.extractNextMessage();
	assert(msg1 == "NICK alice");
	
	// Extract second message
	assert(client.hasCompleteMessage() == true);
	std::string msg2 = client.extractNextMessage();
	assert(msg2 == "USER alice 0 * :Alice");
	
	// Third message is incomplete
	assert(client.hasCompleteMessage() == false);

	std::cout << "  ✓ client_multiple_messages tests passed" << std::endl;
}

// Test write buffer management
void test_client_write_buffer()
{
	Client client(1);
	
	// Initially no unsent data
	assert(client.hasUnsentData() == false);
	
	// Append data to write buffer and check status
	client.appendToWriteBuffer("Welcome to IRC\r\n");
	assert(client.hasUnsentData() == true);

	std::cout << "  ✓ client_write_buffer tests passed" << std::endl;
}

// Test channel membership management
void test_client_channels()
{
	Client client(1);
	
	// Initially client is not in any channels
	assert(client.isInChannel("#general") == false);
	assert(client.getChannels().empty() == true);
	
	// Join channels and check membership
	client.addChannel("#general");
	client.addChannel("#random");
	
	assert(client.isInChannel("#general") == true);
	assert(client.isInChannel("#random") == true);
	assert(client.getChannels().size() == 2);
	
	// Leave a channel and check membership
	client.leaveChannel("#general");
	assert(client.isInChannel("#general") == false);
	assert(client.getChannels().size() == 1);

	std::cout << "  ✓ client_channels tests passed" << std::endl;
}

// Test channel limit constant
void test_client_channel_limit()
{
	Client client(1);
	
	// Check default channel limit
	assert(client.getChannelLimit() == CHANNEL_LIMIT);

	std::cout << "  ✓ client_channel_limit tests passed" << std::endl;
}

void run_client_tests()
{
	std::cout << "\n[Client Tests]" << std::endl;
	test_client_basic();
	test_client_registration();
	test_client_buffers();
	test_client_multiple_messages();
	test_client_write_buffer();
	test_client_channels();
	test_client_channel_limit();
}
