#include <cassert>
#include <string>
#include <vector>
#include <iostream>

bool isPortValid(char *portNumber);
bool isPassValid(const std::string& pass);
std::string addCRLF(const std::string& msg);
std::vector<std::string> splitTargets(const std::string& targets);

void test_isPortValid()
{
	char port1[] = "6667";
	char port2[] = "1024";
	char port3[] = "65535";
	char port4[] = "1023";
	char port5[] = "65536";
	char port6[] = "999999";
	char port7[] = "abcd";

	assert(isPortValid(port1) == true);
	assert(isPortValid(port2) == true);
	assert(isPortValid(port3) == true);
	assert(isPortValid(port4) == false);
	assert(isPortValid(port5) == false);
	assert(isPortValid(port6) == false);
	assert(isPortValid(port7) == false);
	std::cout << "  ✓ isPortValid tests passed" << std::endl;
}

void test_isPassValid()
{
	assert(isPassValid("pass") == true);
	assert(isPassValid("goodPASS123") == true);
	assert(isPassValid("abc") == false);
	assert(isPassValid("with space") == false);
	assert(isPassValid("tab\tpass") == false);
	assert(isPassValid("newline\npass") == false);
	std::cout << "  ✓ isPassValid tests passed" << std::endl;
}

void test_addCRLF()
{
	assert(addCRLF("PING") == "PING\r\n");
	assert(addCRLF("PONG\r\n") == "PONG\r\n");
	std::cout << "  ✓ addCRLF tests passed" << std::endl;
}

void test_splitTargets()
{
	std::vector<std::string> targets = splitTargets("#a, #b ,#c");
	assert(targets.size() == 3);
	assert(targets[0] == "#a");
	assert(targets[1] == "#b");
	assert(targets[2] == "#c");

	targets = splitTargets("  , ,  ");
	assert(targets.empty());
	std::cout << "  ✓ splitTargets tests passed" << std::endl;
}

void run_validation_tests()
{
	std::cout << "\n[Validation Tests]" << std::endl;
	test_isPortValid();
	test_isPassValid();
	test_addCRLF();
	test_splitTargets();
}
