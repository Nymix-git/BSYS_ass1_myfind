cmake_minimum_required(VERSION 3.18.4)

project("myfind")

add_executable(${PROJECT_NAME} ./src/main.c)
add_subdirectory(./src/)

target_link_libraries(${PROJECT_NAME} PUBLIC util)

target_include_directories(${PROJECT_NAME} PUBLIC ./build/ ./src/)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
