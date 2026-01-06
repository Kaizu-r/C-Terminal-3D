# C-Terminal-3D

A fully-featured terminal-based 3D renderer written in C with no external graphics libraries. Renders 3D meshes directly to the Windows console using ASCII characters and depth-based shading.

## ✨ Features

### Core Rendering Pipeline
- **3D Graphics Pipeline**: Complete vertex → view → projection → rasterization pipeline
- **Perspective Projection**: True 3D perspective with configurable FOV, near/far clipping planes
- **Depth Buffer**: Z-buffer implementation for correct depth sorting
- **Backface Culling**: Automatic removal of backward-facing triangles for performance
- **ASCII Shading**: 70+ character gradient for depth-based shading (`$@B%8&WM#*...`)
- **Fragment Rasterization**: Triangle filling with edge detection using Bresenham's line algorithm
- **Optimized Console Output**: Direct console buffer writes using Windows API (WriteConsoleOutput)

### Camera System
- **6-DOF Camera**: Full 6 degrees of freedom movement and rotation
  - **Movement**: WASD (forward/back, strafe), RF (up/down)
  - **Rotation**: IJKL (pitch/yaw), UO (roll)
- **Configurable Parameters**: FOV, near/far planes, movement/rotation speed
- **View Transform**: Camera space transformation with Euler angle rotations

### Mesh Support
- **OBJ File Loader**: Loads 3D models from Wavefront OBJ format
- **Model Transformations**: Scale, rotation (XYZ), and translation
- **Runtime Controls**: 
  - G/H keys to scale mesh in real-time
  - E key to reset mesh position and rotation
- **Included Models**: Bunny, cube, eyeball, sphere

### Performance
- **Real-time Rendering**: Target 24 FPS with frame limiting
- **Live Statistics**: FPS and frame time display in console title
- **Delta Time**: Frame-independent movement and rotation

## 🚀 Quick Start

### Building the Project

**Using GCC (MinGW64 on Windows):**
```bash
gcc Main.c -o C-Terminal-3D.exe
```

**Using VS Code Task (Pre-configured):**
- Press `Ctrl+Shift+B` to build with the default task

### Running

**Run with default model (bunny.obj):**
```bash
.\C-Terminal-3D.exe
```

**List available models:**
```bash
.\C-Terminal-3D.exe --list
```

**Load a specific model:**
```bash
.\C-Terminal-3D.exe --load models/cube_quad.obj
```

**Show help:**
```bash
.\C-Terminal-3D.exe --help
```

### Command-Line Options

| Option | Description | Example |
|--------|-------------|---------|
| *(no arguments)* | Load default model (bunny.obj) | `.\C-Terminal-3D.exe` |
| `--list` | List all .obj files in models directory | `.\C-Terminal-3D.exe --list` |
| `--load <path>` | Load a specific .obj file | `.\C-Terminal-3D.exe --load models/sphere.obj` |
| `--help` or `-h` | Display usage information | `.\C-Terminal-3D.exe --help` |

### Controls

| Key | Action | Category |
|-----|--------|----------|
| **W** | Move camera forward | Camera Movement |
| **S** | Move camera backward | Camera Movement |
| **A** | Move camera left | Camera Movement |
| **D** | Move camera right | Camera Movement |
| **R** | Move camera up | Camera Movement |
| **F** | Move camera down | Camera Movement |
| **I** | Rotate pitch up | Camera Rotation |
| **K** | Rotate pitch down | Camera Rotation |
| **J** | Rotate yaw left | Camera Rotation |
| **L** | Rotate yaw right | Camera Rotation |
| **U** | Roll left | Camera Rotation |
| **O** | Roll right | Camera Rotation |
| **G** | Scale mesh up | Mesh Transform |
| **H** | Scale mesh down | Mesh Transform |
| **E** | Reset mesh position/rotation | Mesh Transform |
| **Q** | Quit application | System |

## 📁 Project Structure

```
C-Terminal-3D/
├── Main.c              # Entry point, main loop, input handling
├── include/
│   ├── camera.h        # Camera system and view transformations
│   ├── coords.h        # Coordinate space conversions
│   ├── frag.h          # Fragment/pixel buffer management
│   ├── list.h          # Linked list for rasterization
│   ├── mesh.h          # OBJ file loader and mesh data structures
│   ├── renderer.h      # Main rendering pipeline and draw calls
│   ├── shader.h        # Line drawing and triangle rasterization (Bresenham)
│   ├── utils.h         # Math utilities, matrices, transformations
│   └── vertex.h        # Vertex and primitive data structures
├── models/
│   ├── bunny.obj       # Stanford bunny model (default)
│   ├── cube_quad.obj   # Cube mesh
│   ├── eyeball.obj     # Eyeball model
│   └── sphere.obj      # Sphere mesh
└── README.md
```

## 🛠️ Technical Details

### Rendering Pipeline
1. **Model Space** → Apply scale, rotation, translation
2. **World Space** → Model positioned in 3D world
3. **View Space** → Transform relative to camera
4. **Clip Space** → Perspective projection with FOV
5. **Screen Space** → Convert to terminal coordinates
6. **Rasterization** → Fill triangles, depth testing
7. **Fragment Shading** → Convert depth to ASCII characters

### Mathematics
- **Rotation Matrices**: 3x3 matrices for X, Y, Z rotations
- **Matrix Multiplication**: Combined transformations
- **Normal Calculation**: Cross product for triangle normals
- **Dot Product**: Used for backface culling
- **Perspective Division**: Depth-based projection

### Requirements

- **OS**: Windows (uses Windows Console API)
- **Compiler**: GCC (MinGW64) or any C compiler
- **Libraries**: Standard C library only (stdio.h, stdlib.h, math.h, windows.h, time.h)

## 🎯 Configuration

Edit constants in [Main.c](Main.c#L9-L15):
```c
#define WIDTH 100              // Terminal width in characters
#define HEIGHT 50              // Terminal height in characters
#define FPS 24                 // Target frames per second
#define CAMERA_ROTATION_SPEED 90.0f  // Degrees per second
#define CAMERA_MOVE_SPEED 5.0f       // Units per second
#define MESH_SCALE_SPEED 5.0f        // Scale factor per second
#define DEFAULT_MODEL "models/bunny.obj"  // Default model when no args
```

To change the default model loaded when running without arguments, modify the `DEFAULT_MODEL` constant.

Alternatively, use command-line arguments to load different models without recompiling:
```bash
.\C-Terminal-3D.exe --load models/sphere.obj
```

## 📝 License

This project is open source and available for educational purposes.
