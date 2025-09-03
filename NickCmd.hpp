#pragma once

#include <iostream>
#include "ACommand.hpp"

class NickCmd : public ACommand
{
	protected:
		std::string _name;

	public:
		NickCmd();
		virtual ~NickCmd();
};