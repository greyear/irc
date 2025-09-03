#pragma once

#include <iostream>
#include "ACommand.hpp"

class UserCmd : public ACommand
{
	protected:
		std::string _name;

	public:
		UserCmd();
		virtual ~UserCmd();
};