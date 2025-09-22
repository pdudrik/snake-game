# snake-game

## Setup
1. Instal gcc dependecies to compile C language
2. Install cmake
- search for cmake file newest version on the web https://cmake.org/download/
'''
sudo apt install cmake
cmake --version
sudo apt install build-essential checkinstall zlib1g-dev libssl-dev -y
wget your-http-cmake-link.tar.gz
tar -zxvf your-http-cmake-link.tar.gz
cd your-http-cmake-folder
./bootstrap
make
sudo make install
'''
3. Install SDL2 library
'''
sudo apt install libsdl2-dev
'''
4. Set CMake file
In project directory
'''
touch CMakeLists.txt
nano CMakeLists.txt
'''

Paste this into file
'''
cmake_minimum_required(VERSION 4.1.1)
project(snake-game)

set(SourceFiles
	main.c)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SourceFiles})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
'''

Then
'''
mkdir build
cd build
cmake ..
make
./snake-game
'''

## Agenda
1. Apply delta time for movement (FPS logic)
2. Improve readings for changing direction, improve movement. It is not smooth.