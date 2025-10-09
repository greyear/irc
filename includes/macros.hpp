#pragma once

// Buffer and size limits
constexpr int MAX_EVENTS = 128;
constexpr int MAX_QUEUE = 128;
constexpr size_t BUFFER_SIZE = 1024;
constexpr size_t MAX_BUFFER_SIZE = 4096;
constexpr size_t CHANNEL_LIMIT = 42;
constexpr size_t MAX_CLIENTS = 1000;

// Flush status codes
constexpr int FLUSH_SUCCESS = 0;
constexpr int FLUSH_ERROR = -1;
constexpr int FLUSH_PARTIAL = 1;
constexpr int FLUSH_LATER = 2;

// Message length limits
constexpr size_t MAX_TOPIC_LENGTH = 300;
constexpr size_t MAX_KICK_REASON  = 200;
constexpr size_t MAX_PART_REASON  = 200;
constexpr size_t MAX_QUIT_REASON  = 200;
constexpr size_t MAX_PRIVMSG_TXT  = 400;
constexpr size_t MAX_CHANNEL_KEY  = 23;

// IRC reply codes
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
