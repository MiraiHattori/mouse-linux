project(boost-serial-test)

cmake_minimum_required(VERSION 3.0)

add_definitions("-Wall -std=c++11")

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
include_directories(Boost::boost)

add_executable(client ./client.cpp reader.cpp)
target_link_libraries(client Boost::system Threads::Threads)
target_compile_options(client PUBLIC -Wall)
target_compile_features(client PUBLIC cxx_std_11)

add_executable(server ./server.cpp writer.cpp)
target_link_libraries(server Boost::system Threads::Threads)
target_compile_options(server PUBLIC -Wall)
target_compile_features(server PUBLIC cxx_std_11)

add_executable(writer ./writer.cpp ./writer_test.cpp)
target_link_libraries(writer Boost::system Threads::Threads)
target_compile_options(writer PUBLIC -Wall)
target_compile_features(writer PUBLIC cxx_std_11)

add_executable(reader ./reader.cpp ./reader_test.cpp)
target_link_libraries(reader Boost::system Threads::Threads)
target_compile_options(reader PUBLIC -Wall)
target_compile_features(reader PUBLIC cxx_std_11)
