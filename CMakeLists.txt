cmake_minimum_required(VERSION 3.22)
project(Snake3D)

set(CMAKE_CXX_STANDARD 20)

include_directories(/home/aidar/Загрузки/emsdk/upstream/emscripten/system/include)

add_executable(Snake3D main.cpp
        src/base/instance.h
        src/base/instance.cpp
        src/base/base.h
        src/base/base.cpp
        src/game/map.cpp
        src/game/map.h
        src/game/Game.cpp
        src/game/Game.h
        src/game/snake.cpp
        src/game/snake.h
        src/game/map.cpp
        test.cpp
        src/base/camera.cpp
        src/base/camera.h)
