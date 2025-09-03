#pragma once

#include <iostream>
#include "ACommand.hpp"

class PassCmd : public ACommand
{
	protected:
		std::string _name;

	public:
		PassCmd();
		virtual ~PassCmd();
};