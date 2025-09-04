#pragma once

#include "UserCmd.hpp"

UserCmd::UserCmd()
{

}

virtual UserCmd::~UserCmd()
{

}

bool UserCmd::needsRegistration() const
{
	return false;
}
