#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "cpp-test.h"

template<typename T>
void draw( const T& x, std::ostream& out, size_t position ) {
	out << std::string( position, ' ' ) << x << std::endl;
}

struct object_t {
	template<typename T>
	object_t( T x ) : self_( std::make_shared<model<T>>( std::move( x ) ) ) {
	}

	friend void draw( const object_t& x, std::ostream& out, size_t position ) {
		x.self_->draw_( out, position );
	}

  private:
	struct concept_t {
		virtual ~concept_t() = default;
		virtual void draw_( std::ostream&, size_t ) const = 0;
	};

	template<typename T>
	struct model final : concept_t {
		model( T x ) : data_( std::move( x ) ) {}

		void draw_( std::ostream& out, size_t position ) const override {
			draw( data_, out, position );
		}

		T data_;
	};

	std::shared_ptr<const concept_t> self_;
};

using document_t = std::vector<object_t>;

void draw( const document_t& x, std::ostream& out, size_t position ) {
	out << std::string( position, ' ' ) << "document {" << std::endl;
	for( const auto& e : x )
		draw( e, out, position + 2 );
	out << std::string( position, ' ' ) << "}" << std::endl;
}

struct my_class_t {
};

void draw( const my_class_t& x, std::ostream& out, size_t position ) {
	out << std::string( position, ' ' ) << "my_class_t" << std::endl;
}

using history_t = std::vector<document_t>;

void commit( history_t& x ) {
	assert( x.size() );
	x.push_back( x.back() );
}

void undo( history_t& x ) {
	assert( x.size() );
	x.pop_back();
}

document_t& current( history_t& x ) {
	assert( x.size() );
	return x.back();
}

int main( int argc, char* argv[] ) {
	auto executable_path = std::filesystem::path( argv[0] );
	std::cout << executable_path.stem().string() << " Version " << cpp_test_VERSION_MAJOR << "."
			  << cpp_test_VERSION_MINOR << std::endl;

	history_t h( 1 );

	current( h ).emplace_back( 0 );
	current( h ).emplace_back( std::string( "Hello!" ) );

	draw( current( h ), std::cout, 0 );
	std::cout << "-------------------------------" << std::endl;

	commit( h );
	current( h )[0] = 42.5;

	auto saving = std::async( [document = current( h )]() {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << "------------- 'save' ------------------" << std::endl;
		draw( document, std::cout, 0 );
	} );

	current( h )[1] = std::string( "World!" );
	current( h ).emplace_back( current( h ) );
	current( h ).emplace_back( my_class_t() );

	draw( current( h ), std::cout, 0 );
	std::cout << "-------------------------------" << std::endl;

	undo( h );

	draw( current( h ), std::cout, 0 );
	std::cout << "-------------------------------" << std::endl;

	return 0;
}