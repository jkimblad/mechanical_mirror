cmake_minimum_required(VERSION 3.17.3)

project(smart_mirror)

add_executable(smart_mirror main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# Opencv
find_package( OpenCV REQUIRED )
if ( OpenCV_FOUND )
  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries (smart_mirror ${OpenCV_LIBS})
endif (OpenCV_FOUND)



