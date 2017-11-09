cmake_minimum_required(VERSION 2.8)

project(OBJ)

include_directories("./")

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
