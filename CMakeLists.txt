cmake_minimum_required(VERSION 3.31)
project(openGL_training)

set(CMAKE_CXX_STANDARD 20)

add_executable(openGL_training
        main.cpp
        src/gl.c
)


# Добавь путь к glad и gl)

# Добавь исходники glad2
add_executable(openGL_training
        main.cpp
        src/gl.c
        shader.h
        stb_image.cpp
        filesystem.h
)

# Линкуй библиотеки
target_link_libraries(openGL_training
        glfw
        dl
        GL
)