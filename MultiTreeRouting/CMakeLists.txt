cmake_minimum_required (VERSION 3.4)

project( MultiTreeRouting)

set (CMAKE_CXX_STANDARD 14)
set (CMake_BUILD_TYPE "Release")

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")

file(GLOB SOURCES "MultiTreeRouting/*.cpp")

find_library( FOUNDATION_LIBRARY Foundation)
find_package(Boost 1.66.0 COMPONENTS graph)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	include_directories(MultiTreeRouting)
	add_executable(MultiTreeRouting ${SOURCES})
	target_link_libraries(MultiTreeRouting ${Boost_LIBRARIES})
endif()