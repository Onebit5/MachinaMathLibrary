# MML Vulkan Examples

## Requirements

### Windows

- **Vulkan SDK** - Download from [vulkan.lunarg.com](https://vulkan.lunarg.com/)
- **CMake 3.16+** - Download from [cmake.org](https://cmake.org/download/)
- **Visual Studio 2022/2026** with C++20 support

### Linux

```bash
# Ubuntu/Debian
sudo apt-get install vulkan-headers libglfw3-dev cmake g++

# Fedora
sudo dnf install vulkan-headers glfw-devel cmake gcc-c++

# Arch Linux
sudo pacman -S vulkan-headers glfw-wayland cmake
```

### All Platforms

- C++20 compatible compiler
- Vulkan capable GPU
- Updated GPU drivers

---

## Building

### Windows

```powershell
# Using Visual Studio Developer Command Prompt
cmake -B build -S .
cmake --build build --config Release

# Run examples
.\build\Release\01_spheres.exe
.\build\Release\02_quaternion_slerp.exe
.\build\Release\03_curves.exe
.\build\Release\04_transforms.exe
.\build\Release\04_free_camera.exe
```

### Linux

```bash
cmake -B build -S .
cmake --build build

# Run examples
./build/01_spheres
./build/02_quaternion_slerp
./build/03_curves
./build/04_transforms
./build/04_free_camera
```

---

## Shader Compilation

The examples use pre-compiled SPIR-V shaders. If you need to modify them:

```bash
glslc shaders/vert.vert -o shaders/vert.spv
glslc shaders/frag.frag -o shaders/frag.spv
```

---

## Troubleshooting

### Black Screen
- Ensure shaders compiled correctly
- Verify Vulkan SDK is properly installed

### Shaders Not Found
- Ensure `.spv` files exist in the `shaders/` directory
- Run from the correct working directory

### Vulkan Not Found
- Set `VULKAN_SDK` environment variable
- Reinstall Vulkan SDK

---

## Licenses

### MML (Machina Math Library)

MIT License - See [LICENSE](../../LICENSE.txt) for details.

### Vulkan

The Vulkan logo and specification are property of The Khronos Group Inc.

> Vulkan is a trademark of The Khronos Group Inc.

### GLFW

Copyright (c) 2002-2006 Marcus Geelnard
Copyright (c) 2006-2019 Camilla Löwy

> GLFW is licensed under the zlib/libpng license.
>
> This license permits use of GLFW in any software, including proprietary and commercial applications, with minimal requirements to retain the copyright notice and license in all copies of the software.

See [GLFW License](https://www.glfw.org/license.html) for full details.

### SPIR-V Tools

Copyright (c) 2015-2023 The Khronos Group Inc.

> Licensed under the Apache License, Version 2.0

See [SPIR-V Tools License](https://github.com/KhronosGroup/SPIRV-Tools?tab=Apache-2.0-1-ov-file) for full details.

---

## Support

For issues with:
- **MML Library**: Check the main repository issues
- **Vulkan SDK**: Visit [vulkan.lunarg.com](https://vulkan.lunarg.com/)
- **GLFW**: Visit [glfw.org](https://www.glfw.org/)