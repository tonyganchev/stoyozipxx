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
	target_link_libraries(stoyozipxx Boost::program_options)
endif()