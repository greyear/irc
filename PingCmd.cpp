#pragma once

#include "PingCmd.hpp"

PingCmd::PingCmd()
{

}

virtual PingCmd::~PingCmd()
{

}

bool PingCmd::needsRegistration() const
{
	return true;
}
