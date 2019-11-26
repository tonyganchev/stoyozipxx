cmake_minimum_required(VERSION 3.14)
project(stoyozipxx)

find_package(Boost COMPONENTS program_options)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(stoyozipxx
	src/main.cpp
	src/compressor.cpp
	src/decompressor.cpp
	src/tester.cpp)
	set_property(TARGET stoyozipxx PROPERTY CXX_STANDARD 17)
	target_compile_options(stoyozipxx PUBLIC "-O3")
	add_executable(test
			src/test.cpp)
			set_property(TARGET test PROPERTY CXX_STANDARD 17)
	target_link_libraries(stoyozipxx Boost::program_options)
else()
	message( FATAL_ERROR "Boost library is required." )
endif()