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

void draw( const std::string& x, std::ostream& out, size_t position ) {
	out << std::string( position, ' ' ) << x << std::endl;
}

struct object_t {
	object_t( int x ) : self_( std::make_unique<int_model_t>( std::move( x ) ) ) {
	}

	object_t( std::string x ) : self_( std::make_unique<string_model_t>( std::move( x ) ) ) {
	}

	object_t( const object_t& x ) : self_( self_->copy_() ) {
	}

	object_t( object_t&& ) noexcept = default;

	object_t& operator=( const object_t& x ) noexcept {
		*this = object_t( x );
		return *this;
	}

	object_t& operator=( object_t&& ) noexcept = default;

	friend void draw( const object_t& x, std::ostream& out, size_t position ) {
		x.self_->draw( out, position );
	}

  private:
	struct concept_t {
		virtual ~concept_t() = default;
		virtual std::unique_ptr<concept_t> copy_() const = 0;
		virtual void draw( std::ostream&, size_t ) const = 0;
	};

	struct int_model_t final : concept_t {
		int_model_t( int x ) : data_( std::move( x ) ) {}

		std::unique_ptr<concept_t> copy_() const override {
			return std::make_unique<int_model_t>( *this );
		}

		void draw( std::ostream& out, size_t position ) const override {
			::draw( data_, out, position );
		}

		int data_;
	};

	struct string_model_t final : concept_t {
		string_model_t( std::string x ) : data_( std::move( x ) ) {}

		std::unique_ptr<concept_t> copy_() const override {
			return std::make_unique<string_model_t>( *this );
		}

		void draw( std::ostream& out, size_t position ) const override {
			::draw( data_, out, position );
		}

		std::string data_;
	};

	std::unique_ptr<concept_t> self_;
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
	document.emplace_back( std::string( "Hello!" ) );
	document.emplace_back( 2 );
	document.emplace_back( 3 );

	std::reverse( document.begin(), document.end() );

	draw( document, std::cout, 0 );

	return 0;
}