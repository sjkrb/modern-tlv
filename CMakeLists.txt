cmake_minimum_required(VERSION 3.5)

project(Modern_TLV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Modern_TLV main.cpp)

target_compile_features(Modern_TLV PRIVATE cxx_std_20)


add_subdirectory(lib)
target_link_libraries(Modern_TLV PRIVATE TLV_LIB)
add_subdirectory(test)

include(GNUInstallDirs)
install(TARGETS Modern_TLV
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
