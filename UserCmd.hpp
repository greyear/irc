#pragma once

#include <iostream>
#include "ACommand.hpp"
#include <regex>

class UserCmd : public ACommand
{
	private:
		std::regex _userNamePattern;
		std::regex _realNamePattern;
	protected:

	public:
		UserCmd();
		virtual ~UserCmd();
		bool needsRegistration() const override;
		bool isUserNameValid(const std::string& userName);
		bool isRealNameValid(const std::string& realName);
		void execute(Server* server, Client* client, const std::vector<std::string>& params, const std::string& multiWordParam) override;

};