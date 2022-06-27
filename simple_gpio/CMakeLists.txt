cmake_minimum_required(VERSION 3.6.1)
project(simple_gpio LANGUAGES CXX)

set(${CMAKE_BUILD_TYPE} Debug)

#GREATER_EQUAL was introduced in cmake 3.7, but yocto 2.2.4 uses cmake 3.6.1
if (VERSION GREATER 3.12 OR VERSION EQUAL 3.12)
    add_compile_definitions(PI4_B)
else ()
    add_definitions(-DPI4_B)

endif()

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpio.h)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)

add_subdirectory(fmt)
add_library(simple_gpio ${HEADERS} ${SOURCES})

set_target_properties(simple_gpio PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries(simple_gpio PRIVATE fmt::fmt-header-only)

