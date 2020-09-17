cmake_minimum_required(VERSION 3.16)

project(smart_mirror)

add_subdirectory (src)
add_subdirectory (lib)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# Opencv
find_package( OpenCV REQUIRED )
if ( OpenCV_FOUND )
  include_directories(${OpenCV_INCLUDE_DIRS})
  target_link_libraries (smart_mirror LINK_PUBLIC ${OpenCV_LIBS} Speeddemon)
endif (OpenCV_FOUND)

install(TARGETS smart_mirror)

