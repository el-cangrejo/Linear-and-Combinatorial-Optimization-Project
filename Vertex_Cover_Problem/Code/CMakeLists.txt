cmake_minimum_required(VERSION 2.8)
project(Graph)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(include)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp")

add_executable(Graph.out ${SOURCES})

target_link_libraries(Graph.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

