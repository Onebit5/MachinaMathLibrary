# Machina Math Library - Manual

A quick reference guide and getting started with MML.

## Getting Started

### Single Header

1. Download or generate `mml.hpp`:
   ```bash
   python scripts/generate_header.py
   ```

2. Copy `mml.hpp` to your project

3. Include in your code:
   ```cpp
   #include "mml.hpp"

   int main() {
       mml::Vector3f pos(1.0f, 2.0f, 3.0f);
       return 0;
   }
   ```

### Header-Only from Source

1. Copy `src/` directory to your include path
2. Include: `#include "mml.hpp"`
3. Use: Start using the math types!

```cpp
#include "mml.hpp"

int main() {
    mml::Vector3f pos(1.0f, 2.0f, 3.0f);
    return 0;
}
```

### CMake Integration

```cmake
add_subdirectory(MachinaMathLibrary)
target_link_libraries(your_target PRIVATE mml)
```

---

## Quick Reference Cheatsheet

### Vector Operations

| Operation | Syntax |
|-----------|--------|
| Create vector | `mml::Vector3f v(x, y, z)` |
| Dot product | `v.dot(w)` or `v & w` |
| Cross product | `v.cross(w)` |
| Length | `v.length()` |
| Normalize | `v.normalized()` |
| Distance | `v.distance(w)` |
| Angle | `v.angle(w)` |
| Projection | `v.project(w)` |
| Add | `v + w` or `v += w` |
| Subtract | `v - w` or `v -= w` |
| Scale | `v * s` or `v *= s` |
| Hadamard | `v.hadamard(w)` |
| Hadamard div | `v.hadamard_div(w)` |
| Component | `v.x`, `v.y`, `v.z`, `v.xyz()` |

### Matrix Operations

| Operation | Syntax |
|-----------|--------|
| Identity | `Matrix4f::identity()` |
| Translation | `Matrix4f::translation(x, y, z)` |
| Rotation X | `Matrix4f::rotation_x(angle)` |
| Rotation Y | `Matrix4f::rotation_y(angle)` |
| Rotation Z | `Matrix4f::rotation_z(angle)` |
| Rotation (axis) | `Matrix4f::rotation(axis, angle)` |
| Scale | `Matrix4f::scale(x, y, z)` |
| Determinant | `m.determinant()` |
| Inverse | `m.inverse()` |
| Transpose | `m.transpose()` |
| Multiply matrix | `m1 * m2` |
| Transform vector | `m * v` |
| Frobenius norm | `m.frobenius_norm()` |
| From Euler | `Matrix3f::from_euler(vec3)` |

### Quaternion Operations

| Operation | Syntax |
|-----------|--------|
| Identity | `Quaternionf::identity()` |
| From axis-angle | `Quaternionf::from_axis_angle(axis, angle)` |
| From Euler | `Quaternionf::from_euler(x, y, z)` |
| From rotation | `Quaternionf::from_rotation_matrix(m)` |
| From-to rotation | `Quaternionf::from_to_rotation(from, to)` |
| Rotation between | `q1.rotation_between(q2)` |
| Slerp | `Quaternionf::slerp(q1, q2, t)` |
| Lerp | `Quaternionf::lerp(q1, q2, t)` |
| Rotate vector | `q.rotate(v)` |
| Conjugate | `q.conjugate()` |
| Inverse | `q.inverse()` |
| To axis-angle | `q.to_axis_angle(axis, angle)` |
| Angle | `q.angle()` |
| Angle to | `q1.angle_to(q2)` |

### Transform Operations

| Operation | Syntax |
|-----------|--------|
| Create | `Transformf::identity()` or `Transformf(pos, rot, scale)` |
| To matrix | `t.to_matrix()` |
| From matrix | `Transformf::from_matrix(m)` |
| Inverse | `t.inverse()` |
| Look at | `Transformf::look_at(eye, target, up)` |
| Transform point | `t.transform_point(v)` |
| Transform direction | `t.transform_direction(v)` |

---

## Code Examples

### Basic Vector Math

```cpp
#include "mml.hpp"

void basic_vectors() {
    // Creation
    mml::Vector3f a(1.0f, 2.0f, 3.0f);
    mml::Vector3f b(4.0f, 5.0f, 6.0f);

    // Arithmetic
    mml::Vector3f sum = a + b;           // (5, 7, 9)
    mml::Vector3f diff = a - b;          // (-3, -3, -3)
    mml::Vector3f scaled = a * 2.0f;     // (2, 4, 6)

    // Dot product
    float dot = a.dot(b);                // 32

    // Cross product
    mml::Vector3f cross = a.cross(b);   // (-3, 6, -3)

    // Length and normalization
    float len = a.length();              // ~3.74
    mml::Vector3f norm = a.normalized(); // (0.27, 0.53, 0.80)

    // Distance
    float dist = a.distance(b);          // ~5.20

    // Angle between
    float angle = a.angle(b);            // ~0.86 radians

    // Projection
    mml::Vector3f proj = a.projected_on(b);

    // Hadamard (element-wise)
    mml::Vector3f h = a.hadamard(b);    // (4, 10, 18)
    mml::Vector3f hd = a.hadamard_div(b); // (0.25, 0.4, 0.5)
}
```

### Matrix Transformations

```cpp
void matrix_transforms() {
    mml::Matrix4f model = mml::Matrix4f::identity();

    // Translate
    model = model * mml::Matrix4f::translation(0.0f, 2.0f, 0.0f);

    // Rotate 45 degrees around Y
    model = model * mml::Matrix4f::rotation_y(0.7854f);

    // Scale
    model = model * mml::Matrix4f::scale(2.0f, 2.0f, 2.0f);

    // Transform a point (homogeneous coordinates)
    mml::Vector4f point(1.0f, 0.0f, 0.0f, 1.0f);
    mml::Vector4f transformed = model * point;

    // Decompose a matrix
    mml::Vector3f translation;
    mml::Quaternionf rotation;
    mml::Vector3f scale;
    model.decompose(translation, rotation, scale);

    // Create from basis vectors
    mml::Vector3f right = mml::Vector3f::unit_x();
    mml::Vector3f up = mml::Vector3f::unit_y();
    mml::Vector3f forward = mml::Vector3f::forward();
    mml::Matrix4f from_basis = mml::Matrix4f::from_basis(right, up, forward);
}
```

### Quaternion Rotations

```cpp
void quaternions() {
    // Create rotation from Euler angles (roll, pitch, yaw)
    mml::Quaternionf q = mml::Quaternionf::from_euler(0.5f, 0.3f, 0.2f);

    // Create from axis-angle
    mml::Vector3f axis = mml::Vector3f::unit_y();
    mml::Quaternionf rot = mml::Quaternionf::from_axis_angle(axis, 1.57f);

    // Rotate a vector
    mml::Vector3f v(1.0f, 0.0f, 0.0f);
    mml::Vector3f rotated = q.rotate(v);

    // Slerp (spherical linear interpolation)
    mml::Quaternionf q1 = mml::Quaternionf::identity();
    mml::Quaternionf q2 = mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_y(), 3.14f);
    mml::Quaternionf slerped = mml::Quaternionf::slerp(q1, q2, 0.5f);

    // Convert back to axis-angle
    mml::Vector3f out_axis;
    float out_angle;
    q.to_axis_angle(out_axis, out_angle);

    // Get angle to another quaternion
    float angle = q1.angle_to(q2);
}
```

### Transform Composition

```cpp
void transforms() {
    mml::Transformf transform;
    transform.position = mml::Vector3f(0.0f, 1.0f, 0.0f);
    transform.rotation = mml::Quaternionf::from_euler(0.0f, 0.785f, 0.0f);
    transform.scale = mml::Vector3f(2.0f, 2.0f, 2.0f);

    // Convert to matrix
    mml::Matrix4f matrix = transform.to_matrix();

    // Transform a point
    mml::Vector3f point(1.0f, 0.0f, 0.0f);
    mml::Vector3f world_point = transform.transform_point(point);

    // Transform a direction (ignores translation)
    mml::Vector3f dir(0.0f, 1.0f, 0.0f);
    mml::Vector3f world_dir = transform.transform_direction(dir);

    // Inverse transform
    mml::Transformf inv = transform.inverse();

    // Look at
    mml::Transformf look = mml::Transformf::look_at(
        mml::Vector3f(0.0f, 0.0f, 5.0f),  // eye
        mml::Vector3f(0.0f, 0.0f, 0.0f), // target
        mml::Vector3f::up()                // up
    );
}
```

### Bezier Curves

```cpp
void bezier_curves() {
    // Quadratic Bezier (3 points)
    mml::BezierQuadraticf quad(
        mml::Vector3f(0, 0, 0),
        mml::Vector3f(1, 1, 0),
        mml::Vector3f(2, 0, 0)
    );

    mml::Vector3f start = quad.evaluate(0.0f);   // (0, 0, 0)
    mml::Vector3f mid = quad.evaluate(0.5f);    // (0.5, 0.5, 0)
    mml::Vector3f end = quad.evaluate(1.0f);    // (2, 0, 0)

    mml::Vector3f tangent = quad.tangent(0.5f);

    // Cubic Bezier (4 points)
    mml::BezierCubicf cubic(
        mml::Vector3f(0, 0, 0),
        mml::Vector3f(1, 1, 0),
        mml::Vector3f(2, 1, 0),
        mml::Vector3f(3, 0, 0)
    );

    float len = cubic.length();
    float t = cubic.reparameterize(0.5f);  // Arc length parameterization

    // Subdivide
    auto [left, right] = cubic.subdivide(0.5f);

    // Flatten for line rendering
    auto segments = cubic.flatten(0.01f);
}
```

### B-Spline and NURBS

```cpp
void splines() {
    std::vector<mml::Vector3f> points = {
        mml::Vector3f(0, 0, 0),
        mml::Vector3f(1, 1, 0),
        mml::Vector3f(2, 1, 0),
        mml::Vector3f(3, 0, 0)
    };

    // B-Spline (degree 3 by default)
    mml::BSplinef spline(points, 3);

    mml::Vector3f p0 = spline.evaluate(0.0f);
    mml::Vector3f p1 = spline.evaluate(0.5f);
    mml::Vector3f pn = spline.evaluate(1.0f);

    auto samples = spline.sample(100);
    float length = spline.length();

    // NURBS with weights (creates conic sections)
    std::vector<float> weights = { 1.0f, 2.0f, 2.0f, 1.0f };
    mml::NURBSf nurbs(points, weights, 3);
    mml::Vector3f weighted = nurbs.evaluate(0.5f);

    // NURBS circle (conic)
    mml::NURBSf circle = mml::NURBSf::circle_2d(1.0f, 32, 2);
    auto circle_pts = circle.sample(100);
}
```

### Catmull-Rom Spline

```cpp
void catmull_rom() {
    std::vector<mml::Vector3f> points = {
        mml::Vector3f(0, 0, 0),
        mml::Vector3f(1, 1, 0),
        mml::Vector3f(2, 1, 0),
        mml::Vector3f(3, 0, 0)
    };

    mml::CatmullRomSplinef spline(points);

    mml::Vector3f pos = spline.evaluate(0.5f);
    mml::Vector3f tan = spline.tangent(0.5f);

    // Get Frenet frame at point
    mml::CurveFrame<float> frame = spline.frame(0.5f);
    mml::Vector3f T = frame.tangent;
    mml::Vector3f N = frame.normal;
    mml::Vector3f B = frame.binormal;

    // Closed loop
    mml::CatmullRomSplinef closed(points, true);

    // Uniform sampling
    auto samples = spline.sample_uniform(10);
}
```

### Spatial Data Structures

```cpp
void spatial_structures() {
    struct MyPrim {
        mml::AABBf bounds;
        int id;
    };

    std::vector<MyPrim> primitives;
    for (int i = 0; i < 100; ++i) {
        MyPrim p;
        p.bounds = mml::AABBf(
            mml::Vector3f(i * 2.0f, 0, 0),
            mml::Vector3f(i * 2.0f + 1, 1, 1)
        );
        p.id = i;
        primitives.push_back(p);
    }

    // BVH
    mml::BVH<float, MyPrim> bvh([](const MyPrim& p) { return p.bounds; });
    bvh.build(primitives);

    auto hits = bvh.ray_intersect(
        mml::Vector3f(-1, 0.5, 0.5),
        mml::Vector3f(1, 0, 0)
    );

    auto found = bvh.point_query(mml::Vector3f(5.5f, 0.5f, 0.5f));

    // Octree
    mml::Octree<float, mml::Spheref> octree(4, 8); // max_depth, max_indices
    octree.insert(sphere1);
    octree.insert(sphere2);

    mml::AABBf query(mml::Vector3f(-1, -1, -1), mml::Vector3f(1, 1, 1));
    auto nearby = octree.aabb_query(query);

    // KD-Tree
    mml::KDTree<float, mml::Spheref> kdtree;
    kdtree.build(spheres);
    auto kd_hits = kdtree.ray_intersect(ray_origin, ray_dir);
}
```

### Transform Hierarchy

```cpp
void transform_hierarchy() {
    mml::TransformNodef root;
    root.set_position(0, 0, 0);
    root.set_rotation(mml::Quaternionf::identity());

    mml::TransformNodef child;
    child.set_position(5, 0, 0);
    child.set_parent(&root);

    mml::TransformNodef grandchild;
    grandchild.set_position(2, 0, 0);
    grandchild.set_parent(&child);

    // Get world matrices
    auto world_root = root.get_world_matrix();
    auto world_child = child.get_world_matrix();
    auto world_grand = grandchild.get_world_matrix();

    // Transform hierarchy batch operations
    std::vector<mml::TransformNodef*> nodes = { &root, &child, &grandchild };
    mml::TransformHierarchyf hierarchy(nodes);
    hierarchy.update_world_matrices();
}
```

### Dual Quaternions (Skeletal Animation)

```cpp
void skeletal_animation() {
    // Single bone as dual quaternion
    mml::DualQuaternionf bone = mml::DualQuaternionf::from_rotation_translation(
        mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_y(), 0.5f),
        mml::Vector3f(0.0f, 1.0f, 0.0f)
    );

    // Skinning matrix
    mml::Matrix4f skin = bone.to_matrix();

    // Interpolate
    mml::DualQuaternionf blended = mml::DualQuaternionf::slerp(bone1, bone2, 0.5f);

    // Full skeleton
    mml::Skeletonf skeleton;

    mml::Bonef root;
    root.position = mml::Vector3f(0, 0, 0);
    root.rotation = mml::Quaternionf::identity();
    root.scale = mml::Vector3f::one();
    skeleton.add_bone("root", root);

    mml::Bonef child;
    child.position = mml::Vector3f(0, 1, 0);
    child.rotation = mml::Quaternionf::from_axis_angle(mml::Vector3f::unit_y(), 0.5f);
    child.scale = mml::Vector3f::one();
    skeleton.add_bone("child", child, "root");

    skeleton.update_matrices();
    auto matrices = skeleton.get_skinning_matrices();
}
```

### Batch Operations (SIMD)

```cpp
void batch_operations() {
    constexpr size_t count = 4;

    mml::Vector4f positions[count] = {
        mml::Vector4f(1, 2, 3, 4),
        mml::Vector4f(2, 3, 4, 5),
        mml::Vector4f(3, 4, 5, 6),
        mml::Vector4f(4, 5, 6, 7)
    };

    mml::Vector4f velocities[count] = {
        mml::Vector4f(0.1f, 0.2f, 0.3f, 0.4f),
        mml::Vector4f(0.1f, 0.2f, 0.3f, 0.4f),
        mml::Vector4f(0.1f, 0.2f, 0.3f, 0.4f),
        mml::Vector4f(0.1f, 0.2f, 0.3f, 0.4f)
    };

    mml::Vector4f results[count];

    // Batch add
    mml::simd::batch::add_arrays(results, positions, velocities, count);

    // Batch lerp
    mml::simd::batch::lerp_arrays(results, positions, velocities, 0.5f, count);

    // Batch dot
    float dots[count];
    mml::simd::batch::dot4_arrays(dots, positions, velocities, count);

    // Individual SIMD
    mml::Vector4f a = positions[0];
    mml::Vector4f b = velocities[0];
    float dot = mml::simd::dot4(a, b);
    float len_sq = mml::simd::length_squared4(a);
}
```

### Geometry and Intersection

```cpp
void geometry() {
    // AABB
    mml::AABBf aabb(mml::Vector3f(0, 0, 0), mml::Vector3f(1, 1, 1));
    mml::Vector3f center = aabb.center();
    float volume = aabb.volume();
    float surface_area = aabb.surface_area();
    bool contains = aabb.contains(mml::Vector3f(0.5f, 0.5f, 0.5f));

    // Sphere
    mml::Spheref sphere(mml::Vector3f(0, 0, 0), 1.0f);
    float vol = sphere.volume();
    float area = sphere.surface_area();

    // Ray intersection
    mml::Rayf ray(mml::Vector3f(-2, 0, 0), mml::Vector3f(1, 0, 0));
    float t;
    if (mml::intersection::ray_sphere(ray, sphere, t)) {
        mml::Vector3f hit = ray.at(t);
    }

    // Capsule
    mml::Capsulef capsule(
        mml::Vector3f(0, 0, 0),
        mml::Vector3f(0, 2, 0),
        0.5f
    );

    // Plane
    mml::Planef plane(mml::Vector3f::unit_y(), 0.0f);
    float dist = plane.distance_to(mml::Vector3f(1, 2, 3));
}
```

### Random Numbers

```cpp
void random() {
    // Non-deterministic
    float r1 = mml::Randomf::value();
    float r2 = mml::Randomf::range(0.0f, 1.0f);
    mml::Vector2f on_circle = mml::Randomf::on_unit_circle();
    mml::Vector3f on_sphere = mml::Randomf::on_unit_sphere();

    // Deterministic (LCG)
    mml::RandomLCG32 lcg(12345);
    uint32_t u = lcg();
    float f = lcg.next_float(0.0f, 1.0f);

    // Fast (Xoroshiro128+)
    mml::RandomXoroshiro128+ fast(42);
    fast.jump();  // Skip ahead
}
```

---

## Type Aliases

| Type | Alias |
|------|-------|
| `Vector2<float>` | `Vector2f` |
| `Vector2<double>` | `Vector2d` |
| `Vector3<float>` | `Vector3f` |
| `Vector3<double>` | `Vector3d` |
| `Vector4<float>` | `Vector4f` |
| `Vector4<double>` | `Vector4d` |
| `Matrix3<float>` | `Matrix3f` |
| `Matrix3<double>` | `Matrix3d` |
| `Matrix4<float>` | `Matrix4f` |
| `Matrix4<double>` | `Matrix4d` |
| `Quaternion<float>` | `Quaternionf` |
| `Quaternion<double>` | `Quaterniond` |
| `DualQuaternion<float>` | `DualQuaternionf` |
| `DualQuaternion<double>` | `DualQuaterniond` |
| `AABB<float>` | `AABBf` |
| `AABB<double>` | `AABBd` |
| `Sphere<float>` | `Spheref` |
| `Sphere<double>` | `Sphered` |
| `BezierCubic<float>` | `BezierCubicf` |
| `BezierCubic<double>` | `BezierCubicd` |
| `BSpline<float>` | `BSplinef` |
| `BSpline<double>` | `BSplined` |
| `NURBS<float>` | `NURBSf` |
| `NURBS<double>` | `NURBSd` |
| `Transform<float>` | `Transformf` |
| `Transform<double>` | `Transformd` |

---

## Common Pitfalls

### 1. Quaternion Multiplication Order

```cpp
// WRONG: q * v applies q to v
mml::Quaternionf q = q2 * q1;  // q2 applied first, then q1
mml::Vector3f v_rotated = q.rotate(v);

// RIGHT: If you want rotation A then B:
mml::Quaternionf combined = qB * qA;
```

### 2. Euler Angle Order

```cpp
// MML uses XYZ order (pitch, yaw, roll)
mml::Quaternionf q = mml::Quaternionf::from_euler(pitch, yaw, roll);
```

### 3. Matrix Multiplication Order

```cpp
// In MML, operator* applies right-to-left
mml::Matrix4f model = scale * rotation * translation;  // T -> R -> S
// This means: translate first, then rotate, then scale
```

### 4. Ray Direction Normalization

```cpp
// Always normalize ray directions
mml::Rayf ray(origin, direction.normalized());
```

### 5. SIMD Availability

```cpp
// Check at runtime
#if MML_USE_SIMD
    // SIMD code
#else
    // Scalar code
#endif
```

---

## Performance Tips

1. **Use SoA for batch operations** - Structure of Arrays is faster for SIMD
2. **Cache transforms** - Don't recalculate matrices every frame
3. **Use BVH for large scenes** - O(log n) vs O(n) for ray queries
4. **Precompute curve samples** - Don't evaluate curves every frame
5. **Use `length_squared()`** - Avoid square root when comparing distances

---

## Further Reading

- See [CHANGELOG.md](CHANGELOG.md) for feature history
- See [README.md](../README.md) for build instructions
- See source code in `src/` for detailed API documentation
