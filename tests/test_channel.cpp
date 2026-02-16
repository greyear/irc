#include <cassert>
#include <string>
#include <iostream>
#include "../includes/Channel.hpp"

// Test updating nicknames
void test_channel_nick_change()
{
	Channel channel("#test");
	
	// Set up users with different roles
	channel.addMember("alice");
	channel.addOperator("alice");
	channel.addInvited("bob");

	// Change alice's nickname to alice2
	channel.updateNickChange("alice", "alice2");
	assert(channel.isMember("alice") == false);
	assert(channel.isMember("alice2") == true);
	assert(channel.isOperator("alice") == false);
	assert(channel.isOperator("alice2") == true);
	
	// Change bob's nickname to bobby
	channel.updateNickChange("bob", "bobby");
	assert(channel.isInvited("bob") == false);
	assert(channel.isInvited("bobby") == true);

	std::cout << "  ✓ channel_nick_change tests passed" << std::endl;
}

// Test invite list management
void test_channel_invites()
{
	Channel channel("#test");
	
	channel.addInvited("guest");
	assert(channel.isInvited("guest") == true);
	assert(channel.isInvited("stranger") == false);
	
	channel.removeInvited("guest");
	assert(channel.isInvited("guest") == false);

	std::cout << "  ✓ channel_invites tests passed" << std::endl;
}

// Test operator and member role management
void test_channel_operator_operations()
{
	Channel channel("#test");
	assert(channel.getName() == "#test");
	
	// Add members with different roles
	channel.addMember("alice");
	channel.addMember("bob");
	channel.addOperator("alice");
	
	// Verify roles
	assert(channel.isOperator("alice") == true);
	assert(channel.isOperator("bob") == false);
	assert(channel.isMember("alice") == true);
	assert(channel.isMember("bob") == true);
	
	// Remove operator status from alice
	channel.removeOperator("alice");
	assert(channel.isOperator("alice") == false);
	assert(channel.isMember("alice") == true);
	
	std::cout << "  ✓ channel operator operations passed" << std::endl;
}

// Test all channel mode combinations (invite-only, topic-restricted, key, limit)
void test_channel_mode_combinations()
{
	Channel channel("#test");
	
	// Default mode is +t (topic restricted)
	std::string modes = channel.getModestring();
	assert(modes == "+t");
	
	// Set invite-only
	channel.setInviteOnly(true);
	modes = channel.getModestring();
	assert(modes == "+it");
	
	// Set key
	channel.setKey("secret");
	modes = channel.getModestring();
	assert(modes == "+kit secret");
	
	// Set limit
	channel.setLimit(5);
	modes = channel.getModestring();
	assert(modes == "+klit secret 5");
	
	// Remove topic restriction
	channel.setTopicRestriction(false);
	modes = channel.getModestring();
	assert(modes == "+kli secret 5");
	
	// Remove key
	channel.removeKey();
	modes = channel.getModestring();
	assert(modes == "+li 5");
	
	// Remove limit
	channel.removeLimit();
	modes = channel.getModestring();
	assert(modes == "+i");
	
	// Remove invite-only
	channel.setInviteOnly(false);
	modes = channel.getModestring();
	assert(modes == "+");
	
	std::cout << "  ✓ channel mode combinations passed" << std::endl;
}

// Test topic setting
void test_channel_topic_operations()
{
	Channel channel("#test");
	channel.setInviteOnly(true);
	assert(channel.getInviteOnly() == true);
	
	// Initially empty topic
	assert(channel.getTopic() == "");
	assert(channel.getTopicSetter() == "");
	assert(channel.getTopicTime() == 0);
	
	// Set topic and verify setter and timestamp
	channel.setTopic("First topic", "alice");
	assert(channel.getTopic() == "First topic");
	assert(channel.getTopicSetter() == "alice");
	assert(channel.getTopicTime() > 0);
	
	time_t firstTime = channel.getTopicTime();
	
	// Update topic and verify changes
	channel.setTopic("Second topic", "bob");
	assert(channel.getTopic() == "Second topic");
	assert(channel.getTopicSetter() == "bob");
	assert(channel.getTopicTime() >= firstTime);
	
	// Clear topic
	channel.setTopic("", "alice");
	assert(channel.getTopic() == "");

	// Remove topic restriction
	channel.setTopicRestriction(false);
	assert(channel.getTopicRestricted() == false);
	
	std::cout << "  ✓ channel topic operations passed" << std::endl;
}

// Test channel member management
void test_channel_members()
{
	Channel channel("#test");
	assert(channel.isEmpty() == true);
	
	// Add multiple members
	channel.addMember("alice");
	channel.addMember("bob");
	channel.addMember("charlie");
	
	// Verify members
	const std::set<std::string>& members = channel.getMembers();
	assert(members.size() == 3);
	assert(members.find("alice") != members.end());
	assert(members.find("bob") != members.end());
	assert(members.find("charlie") != members.end());
	
	// Remove members and verify
	channel.removeMember("bob");
	assert(channel.isMember("bob") == false);
	assert(channel.getMembers().size() == 2);
	
	channel.removeMember("alice");
	channel.removeMember("charlie");
	assert(channel.isEmpty() == true);
	
	std::cout << "  ✓ channel members passed" << std::endl;
}

void run_channel_tests()
{
	std::cout << "\n[Channel Tests]" << std::endl;
	test_channel_nick_change();
	test_channel_invites();
	test_channel_operator_operations();
	test_channel_mode_combinations();
	test_channel_topic_operations();
	test_channel_members();
}
