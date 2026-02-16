#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

bool isPortValid(char *portNumber)
{
	std::regex	pattern("^[0-9]{1,5}$");
	bool		res;

	std::string	portStr = portNumber;
	res = std::regex_match(portStr, pattern);
	if (!res)
		return (false);
	int port = std::atoi(portNumber);
	if (port < 1024 || port > 65535)
		return (false);
	return (true);
}

bool	isPassValid(const std::string& pass)
{
	std::regex pattern("^[^\\x00-\\x1F\\s]{4,64}$");

	return (std::regex_match(pass, pattern));
}

std::string addCRLF(const std::string& msg)
{
	if (msg.size() >= 2 && msg.compare(msg.size() - 2, 2, "\r\n") == 0)
	{
		return msg;
	}
	return msg + "\r\n";
}

std::vector<std::string> splitTargets(const std::string& targets)
{
	std::vector<std::string> result;
	std::stringstream ss(targets);
	std::string eachTarget;
	
	while (std::getline(ss, eachTarget, ','))
	{
		eachTarget.erase(0, eachTarget.find_first_not_of(" \t"));
		eachTarget.erase(eachTarget.find_last_not_of(" \t") + 1);
		
		if (!eachTarget.empty())
			result.push_back(eachTarget);
	}
	return result;
}
