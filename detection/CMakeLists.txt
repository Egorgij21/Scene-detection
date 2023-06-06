add_library(detection detection.cpp include/detection/detection.hpp)

target_link_libraries(detection PRIVATE opencv_core opencv_highgui)

set_property(TARGET detection PROPERTY CXX_STANDARD 20)
target_include_directories(detection PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS detection)