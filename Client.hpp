#pragma once

#include <iostream>
#include <string>
#include <set>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Channel.hpp"
#include "macros.hpp"

class Client
{
	private:
		int						_fd;
		std::string				_nick;
		std::string				_user;
		std::string				_realName;
		std::string				_hostName;
		std::string				_readBuffer;
		std::string				_writeBuffer;
		bool					_hasUnsentData;
		bool					_isEpollOutActive;
		std::set<std::string>	_channels;
		bool					_hasPass;
		bool					_hasUser;
		bool					_hasNick;
		bool					_registered; //TODO: check if we need that
		uint					_channelLimit;
	public:
		Client(int fd);
		~Client();

		//getters
		int								getFd() const;
		const std::string&				getNick() const;
		const std::string&				getUser() const;
		const std::string&				getRealName() const;
		const std::string&				getHostName() const;
		bool							getHasPass() const;
		bool							getHasUser() const;
		bool							getHasNick() const;
		uint								getChannelLimit() const;
		const std::set<std::string>&	getChannels() const;
		std::string						getFullIdentifier() const;

		bool							isRegistered() const;
		bool							hasUnsentData() const;
		bool							isEpollOutActive() const;
		void							setNick(const std::string& nick);
		void							setUser(const std::string& user);
		void							setRealName(const std::string& realName);
		void							setHostName(const std::string& hostName);
		void							setHasPass(bool received);
		void							setHasUser(bool received);
		void							setHasNick(bool received);
		void							setRegistered();
		void							setIsEpollOutActive(bool value);
		bool							checkRegistrationComplete();

		void							appendToReadBuffer(const char* data, size_t length);
		void							appendToWriteBuffer(const std::string& msg);
		bool							hasCompleteMessage() const;
		std::string						extractNextMessage();
		bool							isBufferTooLarge() const;
		void							clearBuffer();
		int								flushWriteBuffer();

		bool							isInChannel(const std::string& channelName) const;
		void							addChannel(const std::string& name);
		void							leaveChannel(const std::string& name);
};
