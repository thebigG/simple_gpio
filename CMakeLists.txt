cmake_minimum_required(VERSION 3.6.1)
project(simple_gpio LANGUAGES CXX)

if (VERSION GREATER_EQUAL 3.12)
    add_compile_definitions(ZEDBOARD)
else ()
    add_definitions(-DZEDBOARD)

endif()

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gpio.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )

add_executable(simple_gpio main.cpp ${HEADERS})
add_subdirectory(fmt)
target_link_libraries(simple_gpio PRIVATE fmt::fmt-header-only Boost::program_options)
