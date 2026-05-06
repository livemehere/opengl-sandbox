# OpenGL Sandbox

https://github.com/user-attachments/assets/f5c69c9b-55e3-4b1e-83db-b12858524093

I started this project while following [LearnOpenGL](https://learnopengl.com/) because I wanted to stop reading graphics code like magic and actually build the pieces myself.

This repo is where I tried things one by one: opening a window, loading GLAD, drawing a textured cube, adding a movable camera, pushing a tiny ECS structure into the loop, and finally loading a model through Assimp.

It is not trying to be a full engine. It is mostly a personal sandbox where I can keep the rendering path small enough to understand and still have enough structure to grow it later.

## What is in here

- GLFW + GLAD setup
- basic shader / texture pipeline
- cube rendering with lighting
- simple camera controls
- tiny ECS-style entity/component setup
- model loading with Assimp
- quick ImGui controls for poking transforms at runtime

Right now the playground renders a few rotating cubes, a small light cube, and a loaded model from the `assets` folder.

## Build

This project uses CMake presets and expects `vcpkg` through `VCPKG_ROOT`.

```bash
cmake --preset debug
cmake --build --preset debug
./build/debug/playground/playground
```

## What after?

I realized that I need to build a solid foundation in math, especially linear algebra.   
Back when I was drawing on a 2D canvas, trigonometry was all I needed lol.   
I managed to follow along with the development up to a certain point, but the moment lighting came into play, it became really hard to wrap my head around.   
So, I've decided to put 3D development on hold for a while. I plan to gain some broader experiences first and then dive deep into it again later.   
For now, I'm off to play around with some IoT stuff! ✌️   
