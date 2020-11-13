#include <iostream>

#include "cpp-test.h"

int main( int argc, char *argv[] ) {
	std::cout << argv[0] << " Version " << cpp_test_VERSION_MAJOR << "."
			  << cpp_test_VERSION_MINOR << std::endl;

	std::cout << "Hello world!" << std::endl;

	return 0;
}