#pragma once

#include "PrivMsgCmd.hpp"

PrivMsgCmd::PrivMsgCmd()
{

}

virtual PrivMsgCmd::~PrivMsgCmd()
{

}

bool PrivMsgCmd::needsRegistration() const
{
	return true;
}
