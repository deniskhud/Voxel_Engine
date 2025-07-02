cmake_minimum_required(VERSION 3.15)
project(openGL_training)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти GLFW
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(openGL_training
        main.cpp
        shader.h
        glad/include/glad/glad.h
        #stb_image.cpp  # если есть
        #shader.cpp     # если есть
)

target_include_directories(openGL_training PRIVATE
        ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(openGL_training
        glfw
        OpenGL::GL
)
