# Machina Math Library (MML)

A modern, header-only C++20 math library for game engines and real-time graphics applications. Provides comprehensive support for vectors, matrices, quaternions, spatial data structures, curves, and transformations.

## Features

### Core Algebra
- **Vectors**: Vector2, Vector3, Vector4 with full operator support
- **Matrices**: Matrix3, Matrix4 with inverse, transpose, decomposition
- **Quaternions**: Rotation, slerp, axis-angle conversion, Euler angles
- **Dual Quaternions**: Skeletal animation and skinning support

### Geometry
- **Primitives**: AABB, Sphere, Capsule, Cylinder, Cone, Plane, Ray, Triangle
- **Intersection Testing**: Ray-AABB, Ray-Sphere, Capsule-AABB, and more
- **Bounding Volumes**: Merge, contain, intersect operations

### Spatial Data Structures
- **BVH**: Bounding Volume Hierarchy for efficient spatial queries
- **Octree**: 8-way spatial partitioning for large scenes
- **KD-Tree**: k-d tree for ray tracing and point queries

### Curves
- **Bezier**: Quadratic and Cubic Bezier curves
- **B-Spline**: General B-spline curves with configurable degree
- **NURBS**: Non-Uniform Rational B-Splines for conic sections
- **Catmull-Rom**: Smooth interpolation through points
- **Hermite**: Tangent-controlled spline curves

### Transformations
- **Transform**: Position, rotation, scale composition
- **TransformNode**: Hierarchical scene graph with dirty propagation
- **Basis**: Orthonormal basis creation and manipulation

### Utilities
- **Angles**: Degree/radian conversion, normalization
- **Interpolation**: Lerp, smoothstep, ease functions
- **Random**: Deterministic and non-deterministic generators
- **SoA**: Structure of Arrays for batch operations
- **SIMD**: Vectorized operations using SSE/AVX (optional)

## Requirements

- **C++20** compatible compiler (GCC 12+, Clang 14+, MSVC 19.29+)
- **CMake** 3.16 or higher
- **SIMD**: Requires CPU with SSE4.1 support (optional, enabled via CMake)

## Installation

### Single Header (Recommended for simple projects)

The easiest way to use MML is to download the single-header file:

1. Download `mml.hpp` from the [releases page](https://github.com/yourusername/MachinaMathLibrary/releases) or generate it:
   ```bash
   python scripts/generate_header.py
   ```

2. Copy `mml.hpp` to your project

3. Include in your code:
   ```cpp
   #include "mml.hpp"
   ```

### Quick Start (Header-Only from source)

Copy the `src/` directory to your project:

```bash
cp -r src /your/project/include/mml
```

Then include in your code:

```cpp
#include "mml.hpp"
```

### CMake Integration

Add to your `CMakeLists.txt`:

```cmake
add_subdirectory(path/to/MachinaMathLibrary)
target_link_libraries(your_target PRIVATE mml)
```

### Installation via CMake

```bash
cmake -B build -S . -DMML_USE_SIMD=ON
cmake --install build --prefix /usr/local
```

## Building

### Configure

```bash
cmake -B build -S . [options]
```

#### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `MML_USE_SIMD` | `ON` | Enable SIMD vectorization (SSE4.1+) |
| `MML_BUILD_TESTS` | `OFF` | Build test suite |
| `MML_BUILD_BENCHMARKS` | `OFF` | Build benchmarks |

### Build

```bash
cmake --build build [--config Release]
```

### Run Tests

```bash
cmake --build build --target run_tests
# Or run directly:
./build/Release/MML_tests.exe          # Windows
./build/Release/MML_tests              # Linux
```

### Build Examples

```bash
cmake -B build -S . -DMML_BUILD_EXAMPLES=ON
cmake --build build
```

## Project Health

| Status | Badge |
|--------|-------|
| Tests | ![Tests](https://img.shields.io/badge/tests-402%20passing-green) |
| Build | ![Build](https://img.shields.io/badge/build-passing-green) |

## Usage Examples

### Vectors

```cpp
#include "mml.hpp"

mml::Vector3f position(1.0f, 2.0f, 3.0f);
mml::Vector3f velocity(0.5f, 1.0f, 0.0f);

float dot = position.dot(velocity);
mml::Vector3f cross = position.cross(velocity);
mml::Vector3f normalized = position.normalized();
```

### Matrices

```cpp
mml::Matrix4f model = mml::Matrix4f::identity();
model = model * mml::Matrix4f::translation(1.0f, 0.0f, 0.0f);
model = model * mml::Matrix4f::rotation_y(mml::Constantsf::half_pi);
model = model * mml::Matrix4f::scale(2.0f, 2.0f, 2.0f);

mml::Vector4f transformed = model * mml::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
```

### Quaternions

```cpp
mml::Quaternionf rotation = mml::Quaternionf::from_euler(0.5f, 0.3f, 0.2f);
mml::Quaternionf slerped = mml::Quaternionf::slerp(q1, q2, 0.5f);

mml::Vector3f axis;
float angle;
rotation.to_axis_angle(axis, angle);
```

### Transforms

```cpp
mml::Transformf transform;
transform.position = mml::Vector3f(0.0f, 1.0f, 0.0f);
transform.rotation = mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_y(), 1.57f);
transform.scale = mml::Vector3f(2.0f, 2.0f, 2.0f);

mml::Matrix4f matrix = transform.to_matrix();
mml::Transformf inverse = transform.inverse();
```

### Curves

```cpp
// Cubic Bezier
mml::BezierCubicf bezier(
    mml::Vector3f(0, 0, 0),
    mml::Vector3f(1, 1, 0),
    mml::Vector3f(2, 1, 0),
    mml::Vector3f(3, 0, 0)
);
mml::Vector3f point = bezier.evaluate(0.5f);

// B-Spline
mml::BSplinef spline(control_points, degree=3);
auto samples = spline.sample(100);

// NURBS Circle
mml::NURBSf circle = mml::NURBSf::circle_2d(1.0f, 32, 2);
```

### Spatial Queries

```cpp
// BVH
mml::BVH<float, MyPrim> bvh([](const MyPrim& p) { return p.bounds; });
bvh.build(primitives);
auto hits = bvh.ray_intersect(ray_origin, ray_direction);

// Octree
mml::Octree<float, Sphere> octree;
octree.insert(sphere1);
octree.insert(sphere2);
auto nearby = octree.aabb_query(search_bounds);
```

### Dual Quaternions (Skeletal Animation)

```cpp
mml::DualQuaternionf dq = mml::DualQuaternionf::from_rotation_translation(
    mml::Quaternionf::identity(),
    mml::Vector3f(0.0f, 1.0f, 0.0f)
);

// Skinning
mml::Matrix4f skin_matrix = dq.to_matrix();

// Skeleton
mml::Skeletonf skeleton;
skeleton.add_bone("root", bone0);
skeleton.add_bone("child", bone1, "root");
```

## Performance

MML includes optional SIMD optimizations for batch operations:

```cpp
constexpr size_t count = 4;
mml::Vector4f positions[count] = { ... };
mml::Vector4f velocities[count] = { ... };
mml::Vector4f results[count];

mml::simd::batch::add_arrays(results, positions, velocities, count);
mml::simd::batch::lerp_arrays(results, positions, velocities, 0.5f, count);
```

Enable SIMD with:
```cmake
cmake -B build -DMML_USE_SIMD=ON
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

MIT License - see [LICENSE](LICENSE.txt) for details.

## Version

Current version: **v1.0.0**

See [CHANGELOG](docs/CHANGELOG.md) for release history.
