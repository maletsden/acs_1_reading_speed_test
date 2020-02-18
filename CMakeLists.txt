cmake_minimum_required( VERSION 3.12 )
project( acs_1_reading_speed_test )

set( CMAKE_CXX_STANDARD 17 )

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # for Clang and AppleClang
    set( CMAKE_CXX_FLAGS "-Ofast" )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # for GCC
    set( CMAKE_CXX_FLAGS "-O3 -Wall -Wextra" )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # for Intel ++
    set( CMAKE_CXX_FLAGS "-fast" )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # for MSVC
    set( CMAKE_CXX_FLAGS "-O2" )
endif()


FIND_PACKAGE( Boost 1.63.0 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


add_executable( test_file_read main.cpp file_readers.cpp words_count.cpp )

add_executable( experiments experiments.cpp file_readers.cpp words_count.cpp test.cpp )

TARGET_LINK_LIBRARIES( test_file_read ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( experiments ${Boost_LIBRARIES} )