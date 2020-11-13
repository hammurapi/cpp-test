#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "cpp-test.h"

void draw( const int& x, std::ostream& out, size_t position ) {
	out << std::string( position, ' ' ) << x << std::endl;
}

struct object_t {
	object_t( const int& x ) : self_( x ) {}

	friend void draw( const object_t& x, std::ostream& out, size_t position ) {
		draw( x.self_, out, position );
	}

  private:
	int self_;
};

using document_t = std::vector<object_t>;

void draw( const document_t& x, std::ostream& out, size_t position ) {
	out << std::string( position, ' ' ) << "document {" << std::endl;
	for( const auto& e : x )
		draw( e, out, position + 2 );
	out << std::string( position, ' ' ) << "}" << std::endl;
}

int main( int argc, char* argv[] ) {
	auto executable_path = std::filesystem::path( argv[0] );
	std::cout << executable_path.stem().string() << " Version " << cpp_test_VERSION_MAJOR << "."
			  << cpp_test_VERSION_MINOR << std::endl;

	document_t document;

	document.emplace_back( 0 );
	document.emplace_back( 1 );
	document.emplace_back( 2 );
	document.emplace_back( 3 );

	draw( document, std::cout, 0 );

	return 0;
}