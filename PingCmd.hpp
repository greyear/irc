#pragma once

#include <iostream>
#include "ACommand.hpp"

class PingCmd : public ACommand
{
	protected:
		std::string _name;

	public:
		PingCmd();
		virtual ~PingCmd();
};