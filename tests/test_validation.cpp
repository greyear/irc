#include <cassert>
#include <string>
#include <vector>
#include <iostream>

bool isPortValid(char *portNumber);
bool isPassValid(const std::string& pass);
std::string addCRLF(const std::string& msg);
std::vector<std::string> splitTargets(const std::string& targets);

// Test port validation for valid and invalid port numbers
void test_isPortValid()
{
	// Valid ports
	char port1[] = "6667";
	char port2[] = "1024";
	char port3[] = "65535";
	// Invalid ports
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

// Test password validation for valid and invalid passwords
void test_isPassValid()
{
	// Valid passwords
	assert(isPassValid("pass") == true);
	assert(isPassValid("goodPASS123") == true);
	// Invalid passwords
	assert(isPassValid("abc") == false);
	assert(isPassValid("with space") == false);
	assert(isPassValid("tab\tpass") == false);
	assert(isPassValid("newline\npass") == false);
	std::cout << "  ✓ isPassValid tests passed" << std::endl;
}

// Test CRLF addition to messages
void test_addCRLF()
{
	assert(addCRLF("PING") == "PING\r\n");
	assert(addCRLF("PONG\r\n") == "PONG\r\n");
	std::cout << "  ✓ addCRLF tests passed" << std::endl;
}

// Test splitting comma-separated targets
void test_splitTargets()
{
	// Normal case with multiple targets
	std::vector<std::string> targets = splitTargets("#a, #b ,#c");
	assert(targets.size() == 3);
	assert(targets[0] == "#a");
	assert(targets[1] == "#b");
	assert(targets[2] == "#c");

	// Case with empty targets and extra commas
	targets = splitTargets("  , ,  ");
	assert(targets.empty());
	std::cout << "  ✓ splitTargets tests passed" << std::endl;
}

// Test edge cases: empty strings, special characters, various spacing
void test_splitTargets_edge_cases()
{
	// Empty string
	std::vector<std::string> targets = splitTargets("");
	assert(targets.empty());
	
	// Only commas and whitespace
	targets = splitTargets(",,,");
	assert(targets.empty());
	
	// Only whitespace
	targets = splitTargets("   ");
	assert(targets.empty());
	
	// Mixed valid and empty targets
	targets = splitTargets("#a,,#b");
	assert(targets.size() == 2);
	assert(targets[0] == "#a");
	assert(targets[1] == "#b");
	
	// Targets with leading/trailing whitespace
	targets = splitTargets("  #a  ,  #b  ,  #c  ");
	assert(targets.size() == 3);
	assert(targets[0] == "#a");
	assert(targets[1] == "#b");
	assert(targets[2] == "#c");
	
	// Targets with special characters in names
	targets = splitTargets("user-,user!,##channel");
	assert(targets.size() == 3);
	assert(targets[0] == "user-");
	assert(targets[1] == "user!");
	assert(targets[2] == "##channel");
	
	std::cout << "  ✓ splitTargets edge cases passed" << std::endl;
}

// Test CRLF handling with various input patterns
void test_buffer_edge_cases()
{
	// Already has CRLF
	assert(addCRLF("MSG\r\n") == "MSG\r\n");
	
	// Ends with just CR
	assert(addCRLF("MSG") == "MSG\r\n");
	
	// Ends with just LF
	assert(addCRLF("MSG\r") == "MSG\r\r\n");
	
	// Ends with CRLF but has extra whitespace
	assert(addCRLF("MSG\n") == "MSG\n\r\n");
	
	// Empty string case
	assert(addCRLF("") == "\r\n");
	
	std::cout << "  ✓ buffer edge cases passed" << std::endl;
}

// Test password validation boundaries and special characters
void test_validation_edge_cases()
{
	// Minimum valid length (4 chars)
	assert(isPassValid("pass") == true);
	assert(isPassValid("1234") == true);
	
	// Below minimum length
	assert(isPassValid("abc") == false);

	// Contains whitespace
	assert(isPassValid("with space") == false);
	
	// Maximum valid length (64 chars)
	std::string maxPass(64, 'a');
	assert(isPassValid(maxPass) == true);
	
	// Above maximum length
	std::string tooLongPass(65, 'a');
	assert(isPassValid(tooLongPass) == false);
	
	// Contains special characters (should be valid)
	assert(isPassValid("p@ss!#$%") == true);
	
	// Contains control characters (should be invalid)
	assert(isPassValid("pass\x01") == false);
	assert(isPassValid("\x1Fpass") == false);
	
	std::cout << "  ✓ validation edge cases passed" << std::endl;
}

void run_validation_tests()
{
	std::cout << "\n[Validation Tests]" << std::endl;
	test_isPortValid();
	test_isPassValid();
	test_addCRLF();
	test_splitTargets();
	test_splitTargets_edge_cases();
	test_buffer_edge_cases();
	test_validation_edge_cases();
}
