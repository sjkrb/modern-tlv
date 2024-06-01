cmake_minimum_required(VERSION 3.5)

project(TLV_LIB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Boost 1.82.0 REQUIRED system)

add_library(TLV_LIB
    base_tlv.cpp
    base_tlv.hpp
    tlv_connection_req.hpp tlv_connection_req.cpp
    tlv_manager.hpp tlv_manager.cpp
    tlv_connection_res.hpp tlv_connection_res.cpp)

target_compile_features(TLV_LIB PRIVATE cxx_std_20)

target_link_libraries(TLV_LIB
  PRIVATE
  Boost::boost)

include(GNUInstallDirs)
install(TARGETS TLV_LIB
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
