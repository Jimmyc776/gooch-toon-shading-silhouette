# Gooch/Toon Shading with Silhouettes

Non-photorealistic renderer implementing Gooch shading, toon quantization, and hull-based silhouettes for the Stanford Teapot and Bunny. Developed for 6.4400: Introduction to Computer Graphics (Fall 2025, MIT).

## Features
- Phong, Gooch, and Toon shading modes
- Optional black silhouettes via backface hull rendering
- Support for multiple meshes (teapot, bunny)
- Keyboard controls for shading mode, outlines, and model

## Controls
- `1` — Phong shading
- `2` — Gooch shading
- `3` — Toon shading
- `O` — Toggle silhouettes on/off
- `M` — Switch model (teapot ↔ bunny)

## Build Instructions

### Dependencies

This project is intended to be built on top of the official 6.4400 starter framework, which provides:

- The `external/` directory (GLFW, GLAD, GLM, ImGui, etc.)
- The GLOO rendering framework and CMake setup

The `external/` folder is **not** included in this repository (it is large and provided separately by the course staff). To build this project, you should:

1. Obtain the 6.4400 starter code from the course (the same codebase used for the assignments).
2. Place this project’s files into the corresponding project/assignment directory within that starter tree, so that `external/` and the GLOO framework from the starter are present.

You will need:

- C++11 or later
- CMake ≥ 3.x
- An OpenGL 3.3+ capable system

### Steps

From the root of the starter+project tree:
mkdir build
cd build
cmake ..
make
./GoochProject (or the executable name configured in CMake)

## Acknowledgments

- Starter framework from MIT 6.4400 course staff.
- Gooch shading model based on:
  Gooch et al., “A Non-Photorealistic Lighting Model for Automatic Technical Illustration,” SIGGRAPH 1998.
