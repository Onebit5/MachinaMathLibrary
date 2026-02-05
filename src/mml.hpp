/**************************************************************************/
/*  mml.hpp                                                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                         MACHINA MATH LIBRARY                           */
/**************************************************************************/
/* Copyright (c) 2026-present Jose A. Perez                               */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation, to use, copy, modify, merge, publish,              */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE           */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

// Machina Math Library (MML)

// Core
#include "core/config.hpp"
#include "core/constants.hpp"
#include "core/epsilon.hpp"
#include "core/traits.hpp"

// Algebra
#include "algebra/dual_quaternion.hpp"
#include "algebra/matrix3.hpp"
#include "algebra/matrix4.hpp"
#include "algebra/quaternion.hpp"
#include "algebra/vector2.hpp"
#include "algebra/vector3.hpp"
#include "algebra/vector4.hpp"

// Geometry
#include "geometry/aabb.hpp"
#include "geometry/bounding_volume.hpp"
#include "geometry/capsule.hpp"
#include "geometry/cone.hpp"
#include "geometry/cylinder.hpp"
#include "geometry/frustum.hpp"
#include "geometry/intersection.hpp"
#include "geometry/line.hpp"
#include "geometry/plane.hpp"
#include "geometry/polygon.hpp"
#include "geometry/ray.hpp"
#include "geometry/sphere.hpp"
#include "geometry/triangle.hpp"

// Transform
#include "transform/transform.hpp"
#include "transform/transform_node.hpp"

// Curves
#include "curves/bspline.hpp"
#include "curves/curves.hpp"
#include "curves/nurbs.hpp"

// Spatial
#include "spatial/bvh.hpp"
#include "spatial/kdtree.hpp"
#include "spatial/octree.hpp"

// Utils
#include "utils/angle.hpp"
#include "utils/basis.hpp"
#include "utils/color.hpp"
#include "utils/interpolation.hpp"
#include "utils/math.hpp"
#include "utils/projection.hpp"
#include "utils/random.hpp"
#include "utils/random_deterministic.hpp"
#include "utils/soa.hpp"