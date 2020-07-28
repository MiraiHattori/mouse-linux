cmake_minimum_required(VERSION 3.0)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
include_directories(Boost::boost)

add_executable(server ./server.cpp mouse_writer.cpp)
target_link_libraries(server PUBLIC Boost::system Threads::Threads)
target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(server PUBLIC -Wall)
target_compile_features(server PUBLIC cxx_std_11)

add_executable(mouse_writer ./mouse_writer.cpp ./mouse_writer_test.cpp)
target_link_libraries(mouse_writer PUBLIC Boost::system Threads::Threads)
target_include_directories(mouse_writer PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(mouse_writer PUBLIC -Wall)
target_compile_features(mouse_writer PUBLIC cxx_std_11)
