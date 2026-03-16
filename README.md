# MACK – 3D Game Engine

MACK is a modular 3D game engine that is being developed with **C++20** and **OpenGL**, focused on high-performance rendering and a scalable data-oriented architecture.

## 🚀 Technical Highlights
* **Entity Component System (ECS)**: Powered by `EnTT`, ensuring cache-friendly data access and efficient entity management.
* **Modern Rendering Pipeline**:
    * Custom **Phong Shading** implementation with directional and point light support.
    * Support for **Lighting Maps** (Diffuse & Specular) for detailed material rendering.
    * **Texture Caching System**: Optimized resource management to prevent redundant GPU memory uploads.
* **Asset Pipeline**: Integrated **Assimp** for complex 3D model loading and scene hierarchy parsing.

## 🛠 Tech Stack
* **Language:** C++20
* **Graphics API:** OpenGL 3.3+
* **Libraries:** EnTT (ECS), GLM (Math), Assimp (Model Loading), glad/GLFW

## 🏗 Build & Run
```bash
# Clone the repository
git clone [https://github.com/kovertno/Mack](https://github.com/kovertno/Mack)
# Build using CMake
mkdir build && cd build
cmake ..
make
