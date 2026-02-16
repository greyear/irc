#include <cassert>
#include <string>
#include <iostream>
#include "../includes/Client.hpp"
#include "../includes/macros.hpp"

void test_client_basic()
{
	Client client(42);

	assert(client.getFd() == 42);
	assert(client.getNick() == "");
	assert(client.getUser() == "");
	
	client.setNick("alice");
	client.setUser("user123");
	client.setRealName("Alice Smith");
	client.setHostName("localhost");
	
	assert(client.getNick() == "alice");
	assert(client.getUser() == "user123");
	assert(client.getRealName() == "Alice Smith");
	assert(client.getHostName() == "localhost");
	
	std::string fullId = client.getFullIdentifier();
	assert(fullId == "alice!user123@localhost");

	std::cout << "  ✓ client_basic tests passed" << std::endl;
}

void test_client_registration()
{
	Client client(1);
	
	assert(client.getHasPass() == false);
	assert(client.getHasUser() == false);
	assert(client.getHasNick() == false);
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasPass(true);
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasUser(true);
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasNick(true);
	assert(client.checkRegistrationComplete() == true);

	std::cout << "  ✓ client_registration tests passed" << std::endl;
}

void test_client_buffers()
{
	Client client(1);
	
	assert(client.hasCompleteMessage() == false);
	
	client.appendToReadBuffer("NICK alice", 10);
	assert(client.hasCompleteMessage() == false);
	
	client.appendToReadBuffer("\r\n", 2);
	assert(client.hasCompleteMessage() == true);
	
	std::string msg = client.extractNextMessage();
	assert(msg == "NICK alice");
	assert(client.hasCompleteMessage() == false);

	std::cout << "  ✓ client_buffers tests passed" << std::endl;
}

void test_client_multiple_messages()
{
	Client client(1);
	
	client.appendToReadBuffer("NICK alice\r\nUSER alice 0 * :Alice\r\nPING", 38);
	
	assert(client.hasCompleteMessage() == true);
	std::string msg1 = client.extractNextMessage();
	assert(msg1 == "NICK alice");
	
	assert(client.hasCompleteMessage() == true);
	std::string msg2 = client.extractNextMessage();
	assert(msg2 == "USER alice 0 * :Alice");
	
	assert(client.hasCompleteMessage() == false);

	std::cout << "  ✓ client_multiple_messages tests passed" << std::endl;
}

void test_client_write_buffer()
{
	Client client(1);
	
	assert(client.hasUnsentData() == false);
	
	client.appendToWriteBuffer("Welcome to IRC\r\n");
	assert(client.hasUnsentData() == true);

	std::cout << "  ✓ client_write_buffer tests passed" << std::endl;
}

void test_client_channels()
{
	Client client(1);
	
	assert(client.isInChannel("#general") == false);
	assert(client.getChannels().empty() == true);
	
	client.addChannel("#general");
	client.addChannel("#random");
	
	assert(client.isInChannel("#general") == true);
	assert(client.isInChannel("#random") == true);
	assert(client.getChannels().size() == 2);
	
	client.leaveChannel("#general");
	assert(client.isInChannel("#general") == false);
	assert(client.getChannels().size() == 1);

	std::cout << "  ✓ client_channels tests passed" << std::endl;
}

void test_client_channel_limit()
{
	Client client(1);
	
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
