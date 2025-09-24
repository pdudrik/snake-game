cmake_minimum_required(VERSION 3.16)
project(snake-game C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} main.c)

find_package(SDL2 REQUIRED)        # provides target SDL2::SDL2 (and sometimes SDL2::SDL2main)
find_package(SDL2_ttf REQUIRED)    # provides target SDL2_ttf::SDL2_ttf

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
)

# On Windows/MinGW you often also need:
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
# Or define SDL_MAIN_HANDLED if you use a console main:
# target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)

