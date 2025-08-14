# A simply `Voxel Engine` written in C++ with `OpenGL`

## Features:  
- the structure of the world is implemented through the classes - block, chunk and world
- Added a procedural generation of the landscape using the noise function
- Implemented rendering optimizations(Face culling, Frustum culling)
- Used a multithreading for CPU operations(chunk generation, noise calculation)

## Core technology: 
- C++, OpenGL
- multithreading
- optimization algorithms
- data structure
- Perlin noise(fastNoiseLite)
- 
<img width="1908" height="1028" alt="изображение" src="https://github.com/user-attachments/assets/d6caeaa1-95ce-4496-bef3-1fd0c4c46f94" />
<img width="1908" height="1028" alt="изображение" src="https://github.com/user-attachments/assets/0cc2dc3d-9f1e-4ec7-88d3-67c1882eed01" />

## Build and run:  

```bash
 g++ main.cpp glad/src/glad.c -Iglad/include block.cpp chunk.cpp world.cpp -lglfw -ldl -lGL -o main\

```bash
./main
