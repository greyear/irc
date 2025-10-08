#pragma once

#define MAX_EVENTS			128
#define MAX_QUEUE			128
#define MAX_CLIENTS			1000
#define BUFFER_SIZE			1024
#define MAX_BUFFER_SIZE		4096
#define FLUSH_SUCCESS		0
#define FLUSH_ERROR			-1
#define FLUSH_PARTIAL		1
#define FLUSH_LATER			2
#define	CHANNEL_LIMIT		42

constexpr size_t MAX_TOPIC_LENGTH = 300; //TODO ^do we need types for the above?
constexpr size_t MAX_KICK_REASON  = 200;
constexpr size_t MAX_PART_REASON  = 200;
constexpr size_t MAX_QUIT_REASON  = 200;
constexpr size_t MAX_PRIVMSG_TXT  = 400;
constexpr size_t MAX_CHANNEL_KEY  = 23;

#define	RPL_WELCOME			"001"
#define	RPL_YOURHOST		"002"
#define	RPL_CREATED			"003"
#define	RPL_MYINFO			"004"
#define RPL_UMODEIS			"221"
#define RPL_CHANNELMODEIS	"324"
#define RPL_CREATIONTIME	"329"
#define RPL_NOTOPIC			"331"
#define RPL_TOPIC			"332"
#define RPL_TOPICWHOTIME	"333"
#define RPL_INVITING		"341"
#define RPL_NAMREPLY		"353"
#define RPL_ENDOFNAMES		"366"
