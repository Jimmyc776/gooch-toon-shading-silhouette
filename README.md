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
- C++11 or later
- CMake ≥ 3.x
- OpenGL 3.3+
- GLFW, GLAD, GLM, ImGui (included via `external/` or describe how to install)

### Steps
mkdir build
cd build
cmake ..
make
./GoochProject (or other executable name)

## Acknowledgments
- Starter framework from MIT 6.4400 course staff
- Gooch shading model based on:
  Gooch et al., “A Non-Photorealistic Lighting Model for Automatic Technical Illustration,” SIGGRAPH 1998.
