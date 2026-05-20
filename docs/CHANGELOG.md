# Changelog

All notable changes to Machina Math Library are documented here.

## [v1.0.1] - 2026-05-20

### Fixed

- **Critical**: Added missing `Matrix4::inverse()` and `Matrix4::inverse_affine()` methods (both were referenced throughout the codebase but did not exist, causing compilation failures in any code using `TransformNode`)
- **Critical**: Fixed `Math::is_inf()` — was using `value > Constants<T>::infinity` which can never be true; replaced with `std::isinf()`
- **Critical**: Fixed BVH `ray_aabb_intersect_distance()` gated behind `#if MML_USE_SIMD` — the function is not SIMD-specific but was only compiled when SIMD was enabled, breaking `ray_intersect_closest()` with SIMD off
- Fixed `Epsilon<T>::value` truncation — `MML_EPSILON` used `1e-6f` suffix which truncated to float precision for `double`; changed to `1e-6` without suffix for proper type deduction
- Fixed `ray_capsule()` overwriting `out_t`/`out_point` in both fallback paths — both sphere endpoint tests wrote to the same output variables; proper distance comparison with separate scratch variables
- Fixed `transform_node.hpp` missing `#include <cstdint>` for `uint32_t` usage
- Fixed `Float8` struct missing closing `};` in SIMD path
- Fixed `Float8::operator*(float)` using `_mm_set1_ps` instead of `_mm256_set1_ps` 
- Fixed `generate_header.py` missing `#include <memory>` in global includes (caused `std::unique_ptr` unresolved)
- Fixed `generate_header.py` missing SIMD intrinsic headers (`<emmintrin.h>`, `<smmintrin.h>`, `<immintrin.h>`)
- Fixed `[[nodiscard]]` attribute on `void` function `Quaternion::to_axis_angle()`
- Fixed `MML_FORCE_INLINE` on recursive `TransformNode::update_world_matrix()` causing inlining failure

## [v1.0.0] - 2026-02-05

### Added

#### Core Algebra

##### Vectors
- `Vector2<T>` - 2D vector with x, y components
- `Vector3<T>` - 3D vector with x, y, z components
- `Vector4<T>` - 4D vector with x, y, z, w components
- Basic operations: addition, subtraction, multiplication, division
- Dot product: `dot()`, `operator&`
- Cross product: `cross()` (Vector3)
- Length: `length()`, `length_squared()`
- Normalization: `normalized()`, `normalize()`
- Distance: `distance()`, `distance_squared()`
- Projection: `project()`, `projected_on()`
- Angle: `angle()`, `angle_to()`
- Perpendicular: `perpendicular()`
- Hadamard operations: `hadamard()`, `hadamard_div()`
- Pseudo-cross product: `cross()` (Vector2)
- Component access: `xyz()`, `xy()`
- Unit vectors: `unit_x()`, `unit_y()`, `unit_z()`, `up()`, `forward()`, `right()`, `left()`, `backward()`, `zero()`
- SIMD operations in `vector4_simd.hpp`

##### Matrices
- `Matrix3<T>` - 3x3 matrix for rotations and transformations
- `Matrix4<T>` - 4x4 matrix for 3D transformations
- Identity matrix: `identity()`
- Transpose: `transpose()`, `transposed()`
- Inverse: `inverse()`, `inverted()`
- Determinant: `determinant()`
- Decomposition: `decompose()`, `extract_translation()`, `extract_rotation()`, `extract_scale()`
- Frobenius norm: `frobenius_norm()`
- Euler angles: `from_euler()`, `to_euler()`
- Basis creation: `from_basis()`
- Direction vectors: `right_vector_raw()`, `up_vector_raw()`, `forward_vector_raw()`
- Matrix multiplication: `operator*`
- Vector transformation: `operator*` (Vector3, Vector4)

##### Quaternions
- `Quaternion<T>` - Unit quaternion for rotations
- Identity: `identity()`
- From axis-angle: `from_axis_angle()`
- From Euler angles: `from_euler()`
- From rotation matrix: `from_rotation_matrix()`
- From-to rotation: `from_to_rotation()`, `rotation_between()`
- To axis-angle: `to_axis_angle()`
- Rotation to target: `rotation_to()`
- Angle to target: `angle_to()`
- Conjugate: `conjugate()`
- Inverse: `inverse()`
- Slerp: `slerp()`
- Lerp: `lerp()`
- Length: `length()`, `normalize()`
- Rotate vector: `rotate()`
- Dot product: `dot()`
- Forward vector: `forward()`, `up()`, `right()`

##### Dual Quaternions
- `DualQuaternion<T>` - Dual quaternion for skeletal animation
- `Bone<T>` - Bone structure with position, rotation, scale, hierarchy
- `Skeleton<T>` - Full skeleton with bind pose, skinning matrices
- From rotation-translation: `from_rotation_translation()`
- Transform point: `transform_point()`
- Transform direction: `transform_direction()`
- Interpolate: `lerp()`, `slerp()`
- Normalize: `normalize()`, `normalized()`
- Inverse: `inverse()`
- To matrix: `to_matrix()`
- Skeleton management: `add_bone()`, `get_bone()`, `update_matrices()`

#### Geometry

##### Primitives
- `AABB<T>` - Axis-aligned bounding box
- `Sphere<T>` - Sphere with center and radius
- `Capsule<T>` - Line segment with radius
- `Cylinder<T>` - Finite cylinder
- `Cone<T>` - Finite cone
- `Plane<T>` - Plane with normal and distance
- `Ray<T>` - Ray with origin and direction
- `Triangle<T>` - Triangle with three vertices

##### Bounding Volumes
- AABB merging: `merge()`, `union()`
- Sphere merging: `merge()`
- Containment: `contains()`, `is_contained_in()`
- Intersection: `intersects()`
- Distance: `distance_to()`
- Transform: `transformed()`
- Volume: `volume()`
- Surface area: `surface_area()`
- Center: `center()`

##### Intersection Testing
- Ray-AABB: `ray_aabb()`
- Ray-Sphere: `ray_sphere()`
- Ray-Capsule: `ray_capsule()`
- Ray-Plane: `ray_plane()`
- Ray-Triangle: `ray_triangle()`
- AABB-Sphere: `aabb_sphere()`
- Capsule-AABB: `capsule_aabb()`
- Sphere-Sphere: `sphere_sphere()`

#### Spatial Data Structures

##### BVH
- `BVH<T, PrimitiveType>` - Bounding Volume Hierarchy
- Build from primitives: `build()`
- Ray intersection: `ray_intersect()`
- Point query: `point_query()`
- AABB query: `aabb_query()`
- Get bounds: `get_bounds()`
- Node count: `node_count()`
- Custom bounds function via lambda

##### Octree
- `Octree<T, PrimitiveType>` - 8-way spatial partitioning
- Configurable max depth
- Configurable indices per leaf
- Insert primitives: `insert()`
- Ray intersection: `ray_intersect()`
- Point query: `point_query()`
- AABB query: `aabb_query()`
- Clear: `clear()`

##### KD-Tree
- `KDTree<T, PrimitiveType>` - k-dimensional tree
- Automatic longest-axis splitting
- Ray intersection: `ray_intersect()`
- Point query: `point_query()`
- AABB query: `aabb_query()`
- Clear: `clear()`

#### Curves

##### Bezier Curves
- `BezierQuadratic<T>` - Quadratic Bezier (3 control points)
- `BezierCubic<T>` - Cubic Bezier (4 control points)
- Evaluate: `evaluate(t)`
- Tangent: `tangent(t)`
- Derivative: `derivative()`, `second_derivative()`
- Curvature: `curvature(t)`
- Subdivide: `subdivide(t)`
- Flatten: `flatten(tolerance)`
- Length: `length()`, `length_fine()`
- Arc length parameterization: `reparameterize()`

##### B-Splines
- `BSpline<T>` - General B-spline curve
- Basis function: `basis_function(i, p, t)`
- Evaluate: `evaluate(t)`
- Tangent: `tangent(t)`
- Derivative: `derivative()`
- Sample: `sample(count)`
- Length: `length()`
- Automatic knot vector generation

##### NURBS
- `NURBS<T>` - Non-Uniform Rational B-Spline
- Weighted control points
- Rational basis functions
- Factory methods:
  - `circle()` - 3D circle
  - `circle_2d()` - 2D circle
  - `cone()` - Cone surface
  - `sphere()` - Sphere surface
- Evaluate: `evaluate(t)`
- Tangent: `tangent(t)`
- Derivative: `derivative()`
- Sample: `sample(count)`
- Length: `length()`

##### Interpolating Splines
- `CatmullRomSpline<T>` - Catmull-Rom spline
- `HermiteSpline<T>` - Hermite spline
- Closed spline option
- Evaluate: `evaluate(t)`
- Tangent: `tangent(t)`
- Normal: `normal()`
- Frame: `frame(t)`
- Convert between types: `from_catmull_rom()`

#### Transformations

##### Transform
- `Transform<T>` - Combined position, rotation, scale
- Identity: `identity()`
- To matrix: `to_matrix()`
- From matrix: `from_matrix()`
- Inverse: `inverse()`
- Look at: `look_at()`
- Transform point/direction
- Decomposition from matrix

##### Transform Node
- `TransformNode<T>` - Hierarchical transform in scene graph
- Position/rotation/scale getters and setters
- Local/world matrix computation
- Dirty flag propagation
- Child/parent hierarchy
- Translate/rotate methods
- `TransformHierarchy` for batch operations

##### Basis
- `Basis3<T>` - Orthonormal basis
- Create from vectors: `from_forward()`, `from_up()`, `from_right()`
- Orthonormalize: `orthonormalize()`
- Rotation matrices
- Check handedness
- Similarity comparison

#### Utilities

##### Constants
- `Constants<T>` - Mathematical constants
- pi, two_pi, half_pi, golden_ratio, epsilon

##### Angles
- `Angle<T>` - Angle utilities
- Degrees/radians conversion
- Normalization
- Shortest distance

##### Interpolation
- `Interpolation<T>` - Interpolation functions
- Lerp, smoothstep
- Ease in/out
- Smootherstep
- Remap

##### Colors
- `Color<T>` - RGBA color
- Preset colors: red, green, blue, white, black, etc.
- HSV conversion
- Luminance
- Component-wise operations

##### Random
- `Random<T>` - Non-deterministic random
- `Randomf`, `Randomd`
- `RandomLCG32`, `RandomPCG32` - Deterministic generators
- `RandomXoroshiro128+`, `RandomSplitMix64` - Fast generators
- Range, chance, on_unit_circle, on_unit_sphere

##### Structure of Arrays (SoA)
- `Vector3SoA<T>` - SoA for Vector3 arrays
- `Vector4SoA<T>` - SoA for Vector4 arrays
- `BatchTransform<T>` - Batch transform operations
- `BatchAABB<T>` - Batch AABB operations
- Optimized for SIMD batch processing
