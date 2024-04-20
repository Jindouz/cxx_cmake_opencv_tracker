cmake_minimum_required(VERSION 3.16)
project(video_test)

find_package(OpenCV CONFIG REQUIRED)

add_executable(video_test video_test.cpp)
target_link_libraries(video_test PRIVATE ${OpenCV_LIBS})