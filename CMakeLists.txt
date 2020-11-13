cmake_minimum_required(VERSION 3.10)

# set the project name
project( cpp-test VERSION 0.1 )

set( CMAKE_CXX_STANDARD 17 )

configure_file( cpp-test.h.in cpp-test.h )

# sources
file( GLOB SOURCES "*.cpp" "*.h" )

# add the executable
add_executable( ${PROJECT_NAME} ${SOURCES} )

target_include_directories( ${PROJECT_NAME}  PUBLIC ${PROJECT_BINARY_DIR} )