cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(CharServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(Boost_USE_STATIC_LIBS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER build)

find_package(Boost COMPONENTS REQUIRED date_time system log log_setup program_options)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB_RECURSE BEAST_FILES
  ${Boost_INCLUDE_DIRS}/boost/beast/*.hpp
  ${Boost_INCLUDE_DIRS}/boost/beast/*.ipp)

source_group(TREE ${Boost_INCLUDE_DIRS}/boost/beast PREFIX beast FILES ${BEAST_FILES})

file(GLOB APP_FILES
  arg_parser.cpp
  arg_parser.hpp
  main.cpp
  message_parser.cpp
  message_parser.hpp
  server.cpp
  server.hpp
  session.cpp
  session.hpp
  socket_reader.cpp
  socket_reader.hpp
  socket_writer.cpp
  socket_writer.hpp
  boost.hpp)

source_group("" FILES ${APP_FILES})

add_executable(server server.cpp ${APP_FILES} ${BEAST_FILES})
target_link_libraries(server PRIVATE Boost::system Boost::log Boost::log_setup Boost::program_options)

find_package(Threads)
target_link_libraries(server PRIVATE Threads::Threads ${Boost_SYSTEM_LIBRARY})

