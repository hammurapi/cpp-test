cmake_minimum_required(VERSION 3.10)

# set the project name
project( cpp-test )

# sources
file( GLOB SOURCES "*.cpp" "*.h" )

# add the executable
add_executable( ${PROJECT_NAME} ${SOURCES} )