add_executable(detect main.cpp)

target_link_libraries(detect PUBLIC detection)

target_link_libraries(detect PRIVATE opencv_core opencv_highgui)