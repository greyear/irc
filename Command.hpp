#pragma once

#include <iostream>

class Command
{
	protected:
		std::string _name;

	public:
		Command();
		virtual ~Command();
};