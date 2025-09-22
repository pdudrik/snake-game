cmake_minimum_required(VERSION 4.1.1)
project(snake-game)

set(SourceFiles
	main.c)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SourceFiles})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
