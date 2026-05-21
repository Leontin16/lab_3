cmake_minimum_required(VERSION 3.10)
project(P4_Mixed_Main_Imp)

set(CMAKE_CXX_STANDARD 17)

# Передаем компилятору точный путь к папке shared на уровень выше
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../shared)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../shared)

add_executable(main_imp main.cpp solver.cpp)