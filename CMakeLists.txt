cmake_minimum_required(VERSION 2.8)

project(flight-simulator)

include_directories(
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/include"
)

link_directories(
    "${CMAKE_SOURCE_DIR}/lib"
)

add_definitions(
    "-DFREEGLUT_STATIC"
)

add_executable(
    ${PROJECT_NAME}
    "main.cpp"
    "Model/Lexer.cpp"
    "Model/Lexer.h"
    "Model/OBJ.cpp"
    "Model/OBJ.h"
    "Model/Model.cpp"
    "Model/Model.h"
)

target_link_libraries(
    ${PROJECT_NAME}
    "freeglut_static"
    "opengl32"
    "glu32"
    "winmm"
    "gdi32"
)
