# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/

cmake_minimum_required(VERSION 3.20)

project(random-coding
	VERSION 0.1.0
	DESCRIPTION "Small random code pieces"
	HOMEPAGE_URL "https://github.com/joseasoler/random-coding"
	)

# Set the C++ standard used by this project.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Disable non-standard compiler extensions by default.
set(CMAKE_CXX_EXTENSIONS OFF)
# Export compile commands information by default.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Catch2 CONFIG REQUIRED)

include(cmake/clang_tidy.cmake)
include(cmake/warnings.cmake)

add_executable(random-coding
	src/main.cpp # Catch2 main file
	src/20230402.cpp
	)

target_include_directories(random-coding PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(random-coding PRIVATE Catch2::Catch2)
target_compile_options(random-coding PRIVATE ${RANDOM_CODING_WARNING_FLAGS})

include(CTest)
include(Catch)
catch_discover_tests(random-coding)
