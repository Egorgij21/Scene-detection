cmake_minimum_required(VERSION 3.12)
project(MyProject)

set(CMAKE_TOOLCHAIN_FILE Users/egorgij21/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

find_package(OpenCV REQUIRED CONFIG COMPONENTS core highgui imgproc)
add_executable(MyProject main.cpp)
target_link_libraries(MyProject PRIVATE opencv_core opencv_highgui opencv_imgproc)