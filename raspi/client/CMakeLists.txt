cmake_minimum_required(VERSION 3.0)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
include_directories(Boost::boost)

add_executable(client ./client.cpp reader.cpp)
target_link_libraries(client PUBLIC Boost::system Threads::Threads)
target_include_directories(client PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(client PUBLIC -Wall)
target_compile_features(client PUBLIC cxx_std_11)

add_executable(reader ./reader.cpp ./reader_test.cpp)
target_link_libraries(reader PUBLIC Boost::system Threads::Threads)
target_include_directories(reader PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(reader PUBLIC -Wall)
target_compile_features(reader PUBLIC cxx_std_11)
