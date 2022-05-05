cmake_minimum_required(VERSION 3.5)
project(simple_gpio LANGUAGES CXX)
add_executable(simple_gpio main.cpp)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
target_link_libraries(simple_gpio Boost::program_options)


