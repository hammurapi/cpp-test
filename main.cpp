#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cpp-test.h"

void draw( const int& x, std::ostream& out, size_t position ) {
	out << std::string( position, ' ' ) << x << std::endl;
}

struct object_t {
	object_t( const int& x ) : self_( std::make_unique<int_model_t>( x ) ) {
		std::cout << "ctor" << std::endl;
	}

	object_t( const object_t& x ) : self_( std::make_unique<int_model_t>( *x.self_ ) ) {
		std::cout << "copy" << std::endl;
	}

	object_t( object_t&& ) noexcept = default;

	object_t& operator=( const object_t& x ) noexcept {
		object_t tmp( x );
		*this = std::move( tmp );
		return *this;
	}

	object_t& operator=( object_t&& ) noexcept = default;

	friend void draw( const object_t& x, std::ostream& out, size_t position ) {
		x.self_->draw( out, position );
	}

  private:
	struct int_model_t {
		int_model_t( const int& x ) : data_( x ) {}

		void draw( std::ostream& out, size_t position ) {
			::draw( data_, out, position );
		}

		int data_;
	};

	std::unique_ptr<int_model_t> self_;
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
	document.reserve( 5 );

	document.emplace_back( 0 );
	document.emplace_back( 1 );
	document.emplace_back( 2 );
	document.emplace_back( 3 );

	std::reverse( document.begin(), document.end() );

	draw( document, std::cout, 0 );

	return 0;
}