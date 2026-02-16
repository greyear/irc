#include <iostream>

void run_validation_tests();
void run_channel_tests();
void run_client_tests();

int main()
{
	std::cout << "==================================" << std::endl;
	std::cout << "  IRC Server Unit Tests" << std::endl;
	std::cout << "==================================" << std::endl;

	run_validation_tests();
	run_channel_tests();
	run_client_tests();

	std::cout << "\n==================================" << std::endl;
	std::cout << "  ✓ All tests passed!" << std::endl;
	std::cout << "==================================" << std::endl;

	return 0;
}
