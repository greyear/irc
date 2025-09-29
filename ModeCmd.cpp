#include "ModeCmd.hpp"
#include "Client.hpp"
#include "Server.hpp"

//what about users' modes?

ModeCmd::ModeCmd()
{

}

ModeCmd::~ModeCmd()
{

}

bool ModeCmd::needsRegistration() const
{
	return true;
}

