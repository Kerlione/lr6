cmake_minimum_required(VERSION 3.13)
project(lr6)

set(CMAKE_CXX_STANDARD 14)

add_executable(lr6 main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(lr6 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

configure_file(object_data object_data COPYONLY)