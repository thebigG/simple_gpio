cmake_minimum_required(VERSION 3.6.1)
project(simple_gpio LANGUAGES CXX)

set(${CMAKE_BUILD_TYPE} Debug)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/simple_gpio/include/gpio.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/simple_gpio/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )

add_executable(simple_gpio_example main.cpp ${HEADERS})
add_subdirectory(simple_gpio)

target_link_libraries(simple_gpio_example PRIVATE  Boost::program_options simple_gpio)

install(TARGETS simple_gpio_example DESTINATION bin)
