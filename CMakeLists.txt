cmake_minimum_required(VERSION 3.6)
project(Pool2D)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(CMAKE_CXX_FLAGS "-g -std=c++11")
include_directories(/home/daniil/opengl/Box2D_v2.1.2/box/image)
include_directories(/home/daniil/opengl/Box2D_v2.1.2/box/sound)
include_directories(
        ${ZIHM_INCLUDE_DIR})
set(SOURCE_FILES main.cpp Ball.cpp Ball.hpp Table.cpp Table.hpp util.hpp MyContactListener.cpp MyContactListener.hpp
        Player.cpp Player.hpp Game.cpp Game.hpp Cue.cpp Cue.hpp InterfaceManager.cpp InterfaceManager.hpp Menu.cpp Menu.hpp SoundManager.cpp SoundManager.h)
add_library(Box2D STATIC IMPORTED)
add_library(zihm SHARED IMPORTED)
add_library(tgui SHARED IMPORTED)

set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION /home/daniil/opengl/Box2D_v2.1.2/box/libBox2D.a)
set_property(TARGET zihm PROPERTY IMPORTED_LOCATION /usr/local/lib/libzihm.so)
set_property(TARGET tgui PROPERTY IMPORTED_LOCATION /usr/local/lib/libtgui.so)

add_executable(Pool2D ${SOURCE_FILES})
target_link_libraries(Pool2D ${TARGET1} Box2D sfml-graphics sfml-window sfml-system sfml-audio tgui)
