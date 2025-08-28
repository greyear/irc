#pragma once

#include <iostream>

class ACommand
{
	protected:
		std::string _name;

	public:
		ACommand();
		virtual ~ACommand();
};