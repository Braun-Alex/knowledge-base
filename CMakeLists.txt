cmake_minimum_required(VERSION 3.26)
project(knowledge_base)

set(CMAKE_CXX_STANDARD 23)
set(HEADERS functions.h)
set(SOURCES functions.cpp main.cpp)

find_package(PkgConfig REQUIRED)

pkg_check_modules(REDLAND REQUIRED IMPORTED_TARGET redland)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PkgConfig::REDLAND)
