#include <cassert>
#include <string>
#include <iostream>
#include "../includes/Client.hpp"
#include "../includes/macros.hpp"

// Test parsing of complete and incomplete messages from read buffer
void test_client_message_parsing()
{
	Client client(1);
	
	// Single complete message
	client.appendToReadBuffer("NICK alice\r\n", 12);
	assert(client.hasCompleteMessage() == true);
	std::string msg = client.extractNextMessage();
	assert(msg == "NICK alice");
	assert(client.hasCompleteMessage() == false);
	
	// Incomplete message (no CRLF)
	client.appendToReadBuffer("USER test", 9);
	assert(client.hasCompleteMessage() == false);
	
	// Complete it
	client.appendToReadBuffer(" 0 * :Test\r\n", 12);
	assert(client.hasCompleteMessage() == true);
	msg = client.extractNextMessage();
	assert(msg == "USER test 0 * :Test");
	
	std::cout << "  ✓ client message parsing passed" << std::endl;
}

// Test handling of empty messages (just CRLF)
void test_client_empty_messages()
{
	Client client(1);
	
	// Empty message (just CRLF)
	client.appendToReadBuffer("\r\n", 2);
	assert(client.hasCompleteMessage() == true);
	std::string msg = client.extractNextMessage();
	assert(msg == "");
	
	std::cout << "  ✓ client empty messages passed" << std::endl;
}

// Test extraction of messages when multiple CRLFs appear consecutively
void test_client_multiple_crlf()
{
	Client client(1);
	
	// Multiple CRLF in a row
	client.appendToReadBuffer("MSG1\r\n\r\nMSG2\r\n", 16);
	
	assert(client.hasCompleteMessage() == true);
	std::string msg1 = client.extractNextMessage();
	assert(msg1 == "MSG1");
	
	assert(client.hasCompleteMessage() == true);
	std::string msg2 = client.extractNextMessage();
	assert(msg2 == "");
	
	assert(client.hasCompleteMessage() == true);
	std::string msg3 = client.extractNextMessage();
	assert(msg3 == "MSG2");
	
	std::cout << "  ✓ client multiple CRLF passed" << std::endl;
}

// Test handling of long messages within buffer limits
void test_client_long_messages()
{
	Client client(1);
	
	// Build a long message (but under MAX_BUFFER_SIZE)
	std::string longMsg(500, 'X');
	longMsg += "\r\n";
	
	client.appendToReadBuffer(longMsg.c_str(), longMsg.length());
	assert(client.hasCompleteMessage() == true);
	
	std::string extracted = client.extractNextMessage();
	assert(extracted.length() == 500);
	assert(extracted == std::string(500, 'X'));
	
	std::cout << "  ✓ client long messages passed" << std::endl;
}

// Test buffer overflow when data exceeds MAX_BUFFER_SIZE
void test_client_buffer_overflow()
{
	Client client(1);
	
	// Add data up to MAX_BUFFER_SIZE
	std::string largeData(MAX_BUFFER_SIZE - 10, 'A');
	client.appendToReadBuffer(largeData.c_str(), largeData.length());
	assert(client.isBufferTooLarge() == false);
	
	// Add more data to exceed limit
	std::string moreData(20, 'B');
	client.appendToReadBuffer(moreData.c_str(), moreData.length());
	assert(client.isBufferTooLarge() == true);
	
	std::cout << "  ✓ client buffer overflow check passed" << std::endl;
}

// Test channel membership management up to CHANNEL_LIMIT
void test_client_channel_operations()
{
	Client client(1);
	
	// Add channels up to limit
	for (uint i = 0; i < CHANNEL_LIMIT; i++) {
		std::string channelName = "#channel" + std::to_string(i);
		client.addChannel(channelName);
	}
	
	assert(client.getChannels().size() == CHANNEL_LIMIT);
	
	// Check specific channels exist
	assert(client.isInChannel("#channel0") == true);
	assert(client.isInChannel("#channel41") == true);
	assert(client.isInChannel("#nonexistent") == false);
	
	// Remove some channels
	client.leaveChannel("#channel0");
	assert(client.isInChannel("#channel0") == false);
	assert(client.getChannels().size() == CHANNEL_LIMIT - 1);
	
	std::cout << "  ✓ client channel operations passed" << std::endl;
}

// Test all combinations of registration flag states
void test_client_registration_states()
{
	Client client(1);
	
	// All combinations of registration flags
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasPass(true);
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasNick(true);
	assert(client.checkRegistrationComplete() == false);
	
	client.setHasUser(true);
	assert(client.checkRegistrationComplete() == true);
	
	// Reset one flag
	client.setHasNick(false);
	assert(client.checkRegistrationComplete() == false);
	
	std::cout << "  ✓ client registration states passed" << std::endl;
}

// Test full identifier construction (nick!user@host) with progressive updates
void test_client_full_identifier()
{
	Client client(1);
	
	// Before setting values
	std::string id1 = client.getFullIdentifier();
	assert(id1 == "!@");
	
	// After setting nick only
	client.setNick("alice");
	std::string id2 = client.getFullIdentifier();
	assert(id2 == "alice!@");
	
	// After setting user
	client.setUser("asmith");
	std::string id3 = client.getFullIdentifier();
	assert(id3 == "alice!asmith@");
	
	// After setting hostname
	client.setHostName("localhost");
	std::string id4 = client.getFullIdentifier();
	assert(id4 == "alice!asmith@localhost");
	
	std::cout << "  ✓ client full identifier passed" << std::endl;
}

void run_protocol_tests()
{
	std::cout << "\n[Protocol Tests]" << std::endl;
	test_client_message_parsing();
	test_client_empty_messages();
	test_client_multiple_crlf();
	test_client_long_messages();
	test_client_buffer_overflow();
	test_client_channel_operations();
	test_client_registration_states();
	test_client_full_identifier();
}
