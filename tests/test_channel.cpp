#include <cassert>
#include <string>
#include <iostream>
#include "../includes/Channel.hpp"

void test_channel_basic()
{
	Channel channel("#test");

	assert(channel.getName() == "#test");
	assert(channel.isEmpty() == true);

	channel.addMember("alice");
	channel.addMember("bob");
	assert(channel.isMember("alice") == true);
	assert(channel.isMember("bob") == true);
	assert(channel.isMember("celine") == false);
	assert(channel.isEmpty() == false);

	channel.addOperator("alice");
	assert(channel.isOperator("alice") == true);
	assert(channel.isOperator("bob") == false);

	channel.removeMember("bob");
	assert(channel.isMember("bob") == false);
	
	std::cout << "  ✓ channel_basic tests passed" << std::endl;
}

void test_channel_topic()
{
	Channel channel("#test");
	
	channel.setInviteOnly(true);
	assert(channel.getInviteOnly() == true);

	channel.setTopic("hello", "alice");
	assert(channel.getTopic() == "hello");
	assert(channel.getTopicSetter() == "alice");
	assert(channel.getTopicTime() != 0);
	
	channel.setTopicRestriction(false);
	assert(channel.getTopicRestricted() == false);
	
	std::cout << "  ✓ channel_topic tests passed" << std::endl;
}

void test_channel_nick_change()
{
	Channel channel("#test");
	
	channel.addMember("alice");
	channel.addOperator("alice");
	channel.addInvited("bob");

	channel.updateNickChange("alice", "alice2");
	assert(channel.isMember("alice") == false);
	assert(channel.isMember("alice2") == true);
	assert(channel.isOperator("alice") == false);
	assert(channel.isOperator("alice2") == true);
	
	channel.updateNickChange("bob", "bobby");
	assert(channel.isInvited("bob") == false);
	assert(channel.isInvited("bobby") == true);

	std::cout << "  ✓ channel_nick_change tests passed" << std::endl;
}

void test_channel_modes()
{
	Channel channel("#test");
	channel.setKey("secret");
	channel.setLimit(10);

	std::string modes = channel.getModestring();
	assert(modes == "+klt secret 10");
	
	assert(channel.getKeyRequired() == true);
	assert(channel.getChannelKey() == "secret");
	assert(channel.getHasLimit() == true);
	assert(channel.getLimit() == 10);
	
	channel.removeKey();
	assert(channel.getKeyRequired() == false);
	
	channel.removeLimit();
	assert(channel.getHasLimit() == false);

	std::cout << "  ✓ channel_modes tests passed" << std::endl;
}

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

void run_channel_tests()
{
	std::cout << "\n[Channel Tests]" << std::endl;
	test_channel_basic();
	test_channel_topic();
	test_channel_nick_change();
	test_channel_modes();
	test_channel_invites();
}
