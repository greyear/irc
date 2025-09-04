#pragma once

#include "NickCmd.hpp"

NickCmd::NickCmd()
{

}
NickCmd::~NickCmd()
{

}

bool NickCmd::needsRegistration() const
{
	return false;
}
