#  OpenGL Graphics Assignments (1A, 1B, 1C)

This repository includes three Computer Graphics assignments implemented using **OpenGL 3.3**, written in **C++**, and executed on **Linux**.

These assignments explore 2D/3D transformations, camera movement, shading, texture mapping, and **Boolean CSG operations** using vertex and fragment shaders.


##  Projects Overview

###  Assignment 1A – 2D Object Animation
- Renders 2D squares on a 900x900 window
- Manual vertex manipulation (no matrix transformations)
- User interaction: `C` / `U` / `D` keys trigger movement
- Uses vertex/fragment shaders for visual output

**Files:** `Source-1A.cpp`, `ProjectVertexShader.vertexshader`, `ProjectFragmentShader.fragmentshader`

---

###  Assignment 1B – 3D Multi-Object Scene
- Displays 2 parallelepipeds and a cube in 3D space
- Implements camera movement (rotation + zoom)
- User interaction via keys `W/X`, `Q/Z`, `+/-`
- Full matrix transformations using GLM
- Color-coded objects with transparency support

**Files:** `Source-1B.cpp`, `proj1B.pdf`

---

###  Assignment 1C – CSG Boolean Operations
- Performs Boolean operations between 3D solids:
  - Union (`U` key)
  - Intersection (`I`)
  - A - B (`D`)
  - B - A (`F`)
- Dynamic texture mapping on each shape
- Advanced buffer setup with UV unwrapping
- Camera system and transformations

**Files:** `source-1C.cpp`, `proj1c.pdf`, shaders, texture images

---

##  Folder Structure

```
opengl-graphics-assignments/
├── part1A/
│   ├── Source-1A.cpp
│   ├── ProjectVertexShader.vertexshader
│   ├── ProjectFragmentShader.fragmentshader
│   └── proj1A.pdf
├── part1B/
│   ├── Source-1B.cpp
│   └── proj1B.pdf
├── part1C/
│   ├── source-1C.cpp
│   ├── proj1c.pdf
│   ├── P1C.vertexshader
│   ├── P1C.fragmentshader
│   └── textures/
└── README.md
```

---

## 🛠️ Dependencies

- OpenGL 3.3+
- GLEW
- GLFW
- GLM
- stb_image
- Linux environment recommended


---

##  License

This code is for educational use only.  
Do not redistribute or submit as your own.

---

##  Author

**Maria Tsatsari**  
GitHub: [maytsatsari](https://github.com/maytsatsari)
