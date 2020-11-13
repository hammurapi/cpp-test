#include <filesystem>
#include <iostream>

#include "cpp-test.h"

int main( int argc, char *argv[] ) {
	auto executable_path = std::filesystem::path( argv[0] );
	
	std::cout << executable_path.stem().string() << " Version " << cpp_test_VERSION_MAJOR << "."
			  << cpp_test_VERSION_MINOR << std::endl;


	return 0;
}