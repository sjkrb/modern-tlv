cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project(tests)

find_package(Catch2)
# These tests can use the Catch2-provided main

add_executable(tests test_base_tlv.cpp)

target_include_directories(tests PRIVATE ../)
target_link_libraries(tests PRIVATE TLV_LIB Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
