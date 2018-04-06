project( MultiTreeRouting)

cmake_minimum_required (VERSION 3.4)

#
# mulle-bootstrap environment
#

# check if compiling with mulle-bootstrap (works since 2.6)

set (CMAKE_CXX_STANDARD 11)

if( NOT MULLE_BOOTSTRAP_VERSION)
  include_directories( BEFORE SYSTEM
dependencies/include
addictions/include
)

  set( CMAKE_FRAMEWORK_PATH
dependencies/Frameworks
addictions/Frameworks
${CMAKE_FRAMEWORK_PATH}
)

  set( CMAKE_LIBRARY_PATH
dependencies/lib
addictions/lib
${CMAKE_LIBRARY_PATH}
)

set( DEPENDENCIES_DIR dependencies)
set( ADDICTIONS_DIR addictions)

endif()


#
# Platform specific definitions
#

if( APPLE)
   # # CMAKE_OSX_SYSROOT must be set for CMAKE_OSX_DEPLOYMENT_TARGET (cmake bug)
   # if( NOT CMAKE_OSX_SYSROOT)
   #    set( CMAKE_OSX_SYSROOT "/" CACHE STRING "SDK for OSX" FORCE)   # means current OS X
   # endif()
   #
   # # baseline set to 10.6 for rpath
   # if( NOT CMAKE_OSX_DEPLOYMENT_TARGET)
   #   set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6" CACHE STRING "Deployment target for OSX" FORCE)
   # endif()

   set( CMAKE_POSITION_INDEPENDENT_CODE FALSE)

   set( BEGIN_ALL_LOAD "-all_load")
   set( END_ALL_LOAD)
else()
   set( CMAKE_POSITION_INDEPENDENT_CODE TRUE)

   if( WIN32)
   # windows
   else()
   # linux / gcc
      set( BEGIN_ALL_LOAD "-Wl,--whole-archive")
      set( END_ALL_LOAD "-Wl,--no-whole-archive")
   endif()
endif()


##
## MultiTreeRouting Files
##

# uncomment this for mulle-objc to search libraries first
# set( CMAKE_FIND_FRAMEWORK "LAST")

set( SOURCES
MultiTreeRouting/CenterHeuristic.cpp
MultiTreeRouting/GraphLoader.cpp
MultiTreeRouting/MatrixLoader.cpp
MultiTreeRouting/Printer.cpp
MultiTreeRouting/SimpleHeuristic.cpp
MultiTreeRouting/TreeWorker.cpp
MultiTreeRouting/Writer.cpp
MultiTreeRouting/main.cpp
)

set( STATIC_DEPENDENCIES
)

set(BOOST_ROOT usr/local/Cellar/boost/1.66.0)
set(Boost_LIBRARY_DIR usr/local/Cellar/boost/1.66.0/lib)
find_library( FOUNDATION_LIBRARY Foundation)
message( STATUS "FOUNDATION_LIBRARY is ${FOUNDATION_LIBRARY}")
find_library( BOOST_SYSTEM_LIBRARY boost_system)
message( STATUS "BOOST_SYSTEM_LIBRARY is ${BOOST_SYSTEM_LIBRARY}")
find_package(Boost COMPONENTS system graph REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set( DEPENDENCIES
${BOOST_SYSTEM_LIBRARY}
${FOUNDATION_LIBRARY}
)


##
## MultiTreeRouting
##

add_executable( MultiTreeRouting MACOSX_BUNDLE
${SOURCES}
${PUBLIC_HEADERS}
${PROJECT_HEADERS}
${PRIVATE_HEADERS}
${RESOURCES}
)

target_include_directories( MultiTreeRouting
   PUBLIC
)

target_link_libraries( MultiTreeRouting
${BEGIN_ALL_LOAD}
${STATIC_DEPENDENCIES}
${END_ALL_LOAD}
${DEPENDENCIES}
)