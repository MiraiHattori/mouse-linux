cmake_minimum_required(VERSION 3.0)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
include_directories(Boost::boost)

add_executable(client ./client.cpp mouse_reader.cpp)
target_link_libraries(client PUBLIC Boost::system Threads::Threads)
target_include_directories(client PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(client PUBLIC -Wall)
target_compile_features(client PUBLIC cxx_std_11)

add_executable(mouse_reader ./mouse_reader.cpp ./mouse_reader_test.cpp)
target_link_libraries(mouse_reader PUBLIC Boost::system Threads::Threads)
target_include_directories(mouse_reader PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(mouse_reader PUBLIC -Wall)
target_compile_features(mouse_reader PUBLIC cxx_std_11)
