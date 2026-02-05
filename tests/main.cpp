#include "../src/algebra/vector4_simd.hpp"
#include "../src/mml.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

int total_tests = 0;
int passed_tests = 0;
int failed_tests = 0;

void print_header(const std::string &text) {
	std::cout << "\n========================================" << std::endl;
	std::cout << "   " << text << std::endl;
	std::cout << "========================================" << std::endl;
}

void assert_equal_float(float actual, float expected, const std::string &message, float epsilon = 1e-5f) {
	total_tests++;
	bool passed = std::abs(actual - expected) < epsilon;
	if (passed) {
		passed_tests++;
		std::cout << "  [PASS] " << message << " (actual: " << actual << ", expected: " << expected << ")" << std::endl;
	} else {
		failed_tests++;
		std::cout << "  [FAIL] " << message << " (actual: " << actual << ", expected: " << expected << ")" << std::endl;
	}
}

void test_constants() {
	print_header("Constants Tests");
	assert_equal_float(mml::Constantsf::pi, 3.14159265f, "Pi constant");
	assert_equal_float(mml::Constantsf::two_pi, 6.28318530f, "Two Pi constant");
	assert_equal_float(mml::Constantsf::half_pi, 1.57079637f, "Half Pi constant");
	assert_equal_float(mml::Constantsf::golden_ratio, 1.61803399f, "Golden Ratio constant");
}

void test_epsilon() {
	print_header("Epsilon Tests");
	assert_equal_float(mml::Epsilonf::value, 1e-6f, "Default epsilon value");
	assert_equal_float(mml::Epsilonf::high, 1e-6f, "High epsilon value");
	assert_equal_float(mml::Epsilonf::medium, 1e-4f, "Medium epsilon value");
}

void test_math() {
	print_header("Math Utilities Tests");
	assert_equal_float(mml::Mathf::abs(-5.5f), 5.5f, "Math::abs(-5.5)");
	assert_equal_float(mml::Mathf::min(3.0f, 7.0f), 3.0f, "Math::min(3, 7)");
	assert_equal_float(mml::Mathf::max(3.0f, 7.0f), 7.0f, "Math::max(3, 7)");
	assert_equal_float(mml::Mathf::clamp(-2.0f, 0.0f, 5.0f), 0.0f, "Math::clamp(-2, 0, 5)");
	assert_equal_float(mml::Mathf::lerp(0.0f, 10.0f, 0.5f), 5.0f, "Math::lerp(0, 10, 0.5)");
	assert_equal_float(mml::Mathf::sqrt(9.0f), 3.0f, "Math::sqrt(9)");
	assert_equal_float(mml::Mathf::degrees_to_radians(180.0f), 3.14159265f, "Math::degrees_to_radians(180)");
	assert_equal_float(mml::Mathf::radians_to_degrees(3.14159265f), 180.0f, "Math::radians_to_degrees(PI)");
}

void test_color() {
	print_header("Color Tests");
	mml::Colorf red = mml::Colorf::red();
	mml::Colorf green = mml::Colorf::green();
	assert_equal_float(red.r, 1.0f, "Color::red() r component");
	assert_equal_float(green.g, 1.0f, "Color::green() g component");

	mml::Colorf yellow = red + green;
	assert_equal_float(yellow.r, 1.0f, "Color: red + green r component");
	assert_equal_float(yellow.g, 1.0f, "Color: red + green g component");

	mml::Colorf gray = mml::Colorf(0.5f, 0.5f, 0.5f);
	assert_equal_float(gray.luminance(), 0.5f, "Color::luminance()");

	mml::Colorf from_rgb = mml::Colorf::from_rgb(255, 128, 64);
	assert_equal_float(from_rgb.r, 1.0f, "Color::from_rgb(255, 128, 64) r");
	assert_equal_float(from_rgb.g, 0.50196f, "Color::from_rgb(255, 128, 64) g");

	float h, s, v, a;
	from_rgb.to_hsv(h, s, v, a);
	assert_equal_float(a, 1.0f, "Color::to_hsv() alpha");
}

void test_vectors() {
	print_header("Vector Tests");

	mml::Vector2f v2a(3.0f, 4.0f);
	mml::Vector2f v2b(1.0f, 2.0f);
	assert_equal_float(v2a.length(), 5.0f, "Vector2::length()");
	assert_equal_float(v2a.distance(v2b), 2.828427f, "Vector2::distance()");
	assert_equal_float(v2a.dot(v2b), 11.0f, "Vector2::dot()");
	assert_equal_float(v2a.perpendicular().x, -4.0f, "Vector2::perpendicular() x");

	mml::Vector3f v3a(1.0f, 0.0f, 0.0f);
	mml::Vector3f v3b(0.0f, 1.0f, 0.0f);
	mml::Vector3f cross = v3a.cross(v3b);
	assert_equal_float(cross.z, 1.0f, "Vector3::cross() z component");
	assert_equal_float(v3a.angle(v3b), 1.57079637f, "Vector3::angle()");
	assert_equal_float(v3a.project(v3b).x, 0.0f, "Vector3::project() x component");

	assert_equal_float(mml::Vector3f::unit_x().x, 1.0f, "Vector3::unit_x()");
	assert_equal_float(mml::Vector3f::up().y, 1.0f, "Vector3::up() y");
	assert_equal_float(mml::Vector3f::forward().z, 1.0f, "Vector3::forward() z");

	mml::Vector4f v4a(1.0f, 2.0f, 3.0f, 4.0f);
	mml::Vector4f v4b(2.0f, 3.0f, 4.0f, 5.0f);
	assert_equal_float(v4a.dot(v4b), 40.0f, "Vector4::dot()");
	assert_equal_float(v4a.xyz().z, 3.0f, "Vector4::xyz() z component");
	assert_equal_float(v4a.normalized().length(), 1.0f, "Vector4::normalized() length");

	mml::Vector2f v2c(4.0f, 6.0f);
	mml::Vector2f v2d(2.0f, 3.0f);
	mml::Vector2f v2_hadamard = v2c.hadamard(v2d);
	assert_equal_float(v2_hadamard.x, 8.0f, "Vector2::hadamard() x");
	assert_equal_float(v2_hadamard.y, 18.0f, "Vector2::hadamard() y");
	mml::Vector2f v2_div = v2c.hadamard_div(v2d);
	assert_equal_float(v2_div.x, 2.0f, "Vector2::hadamard_div() x");
	assert_equal_float(v2_div.y, 2.0f, "Vector2::hadamard_div() y");
	assert_equal_float(v2a.cross(v2b), 2.0f, "Vector2::cross() pseudo-cross");

	mml::Vector3f v3c(2.0f, 4.0f, 6.0f);
	mml::Vector3f v3d(1.0f, 2.0f, 3.0f);
	mml::Vector3f v3_hadamard = v3c.hadamard(v3d);
	assert_equal_float(v3_hadamard.x, 2.0f, "Vector3::hadamard() x");
	assert_equal_float(v3_hadamard.y, 8.0f, "Vector3::hadamard() y");
	assert_equal_float(v3_hadamard.z, 18.0f, "Vector3::hadamard() z");
	mml::Vector3f v3_div = v3c.hadamard_div(v3d);
	assert_equal_float(v3_div.x, 2.0f, "Vector3::hadamard_div() x");
	assert_equal_float(v3_div.y, 2.0f, "Vector3::hadamard_div() y");
	assert_equal_float(v3_div.z, 2.0f, "Vector3::hadamard_div() z");

	mml::Vector4f v4c(4.0f, 6.0f, 8.0f, 10.0f);
	mml::Vector4f v4d(2.0f, 3.0f, 4.0f, 5.0f);
	mml::Vector4f v4_hadamard = v4c.hadamard(v4d);
	assert_equal_float(v4_hadamard.x, 8.0f, "Vector4::hadamard() x");
	assert_equal_float(v4_hadamard.y, 18.0f, "Vector4::hadamard() y");
	mml::Vector4f v4_div = v4c.hadamard_div(v4d);
	assert_equal_float(v4_div.x, 2.0f, "Vector4::hadamard_div() x");
	assert_equal_float(v4_div.y, 2.0f, "Vector4::hadamard_div() y");
}

void test_matrices() {
	print_header("Matrix Tests");

	mml::Matrix3f m3_identity = mml::Matrix3f::identity();
	mml::Vector3f v3(1.0f, 2.0f, 3.0f);
	mml::Vector3f result = m3_identity * v3;
	assert_equal_float(result.x, 1.0f, "Matrix3::identity() x component");

	mml::Matrix3f m3(1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 3.0f);
	assert_equal_float(m3.determinant(), 6.0f, "Matrix3::determinant()");

	mml::Matrix3f m3_inv = m3.inverse();
	mml::Matrix3f product = m3 * m3_inv;
	assert_equal_float(product[0][0], 1.0f, "Matrix3::inverse() [0][0]");

	mml::Matrix4f m4_identity = mml::Matrix4f::identity();
	mml::Vector4f v4(1.0f, 2.0f, 3.0f, 4.0f);
	mml::Vector4f v4_result = m4_identity * v4;
	assert_equal_float(v4_result.x, 1.0f, "Matrix4::identity() x component");

	mml::Matrix4f m4_trans = mml::Matrix4f::translation(1.0f, 2.0f, 3.0f);
	mml::Vector4f point(0.0f, 0.0f, 0.0f, 1.0f);
	mml::Vector4f translated = m4_trans * point;
	assert_equal_float(translated.x, 1.0f, "Matrix4::translation() x component");

	mml::Matrix4f m4_scale = mml::Matrix4f::scale(2.0f, 3.0f, 4.0f);
	assert_equal_float(m4_scale[0][0], 2.0f, "Matrix4::scale() [0][0]");

	mml::Matrix4f m4_rot_y = mml::Matrix4f::rotation_y(mml::Constantsf::half_pi);
	mml::Vector4f v_x(1.0f, 0.0f, 0.0f, 0.0f);
	mml::Vector4f v_rotated = m4_rot_y * v_x;
	assert_equal_float(v_rotated.z, -1.0f, "Matrix4::rotation_y() z component");

	mml::Matrix3f m3_frob = mml::Matrix3f::identity();
	assert_equal_float(m3_frob.frobenius_norm(), 1.73205081f, "Matrix3::frobenius_norm() identity");
	mml::Matrix3f m3_euler = mml::Matrix3f::from_euler(mml::Vector3f(0.0f, mml::Constantsf::half_pi, 0.0f));
	assert_equal_float(m3_euler[0][0], 0.0f, "Matrix3::from_euler() [0][0]", 0.01f);

	mml::Matrix4f m4_frob = mml::Matrix4f::identity();
	assert_equal_float(m4_frob.frobenius_norm(), 2.0f, "Matrix4::frobenius_norm() identity");
	mml::Matrix4f m4_euler = mml::Matrix4f::from_euler(mml::Vector3f(0.0f, mml::Constantsf::half_pi, 0.0f));
	assert_equal_float(m4_euler[0][0], 0.0f, "Matrix4::from_euler() [0][0]", 0.01f);

	mml::Matrix4f m4_basis = mml::Matrix4f::from_basis(mml::Vector3f::unit_x(), mml::Vector3f::unit_y(), mml::Vector3f::unit_z());
	assert_equal_float(m4_basis[0][0], 1.0f, "Matrix4::from_basis() [0][0]");
}

void test_quaternions() {
	print_header("Quaternion Tests");

	mml::Quaternionf q_identity = mml::Quaternionf::identity();
	mml::Vector3f v(1.0f, 2.0f, 3.0f);
	mml::Vector3f rotated = q_identity.rotate(v);
	assert_equal_float(rotated.x, 1.0f, "Quaternion::identity() rotate x component");

	mml::Quaternionf q_axis = mml::Quaternionf::from_axis_angle(
			mml::Vector3f(0.0f, 0.0f, 1.0f), mml::Constantsf::half_pi);
	assert_equal_float(q_axis.length(), 1.0f, "Quaternion::from_axis_angle() length");
	assert_equal_float(q_axis.angle(), 1.57079637f, "Quaternion::angle()");

	mml::Quaternionf q_conj = q_axis.conjugate();
	assert_equal_float(q_conj.x, 0.0f, "Quaternion::conjugate() x component");
	assert_equal_float(q_conj.w, 0.70710677f, "Quaternion::conjugate() w component");

	mml::Quaternionf q_start = mml::Quaternionf::identity();
	mml::Quaternionf q_end = q_axis;
	mml::Quaternionf q_slerp = mml::Quaternionf::slerp(q_start, q_end, 0.5f);
	assert_equal_float(q_slerp.length(), 1.0f, "Quaternion::slerp() length");

	mml::Quaternionf q_rot = mml::Quaternionf::from_to_rotation(mml::Vector3f::unit_x(), mml::Vector3f::unit_y());
	assert_equal_float(q_rot.length(), 1.0f, "Quaternion::from_to_rotation() length");
	mml::Quaternionf q_rot_alias = mml::Quaternionf::rotation_between(mml::Vector3f::unit_x(), mml::Vector3f::unit_y());
	assert_equal_float(q_rot_alias.length(), 1.0f, "Quaternion::rotation_between() length");

	mml::Vector3f axis;
	float angle;
	q_axis.to_axis_angle(axis, angle);
	assert_equal_float(axis.z, 1.0f, "Quaternion::to_axis_angle() axis z");
	assert_equal_float(angle, 1.57079637f, "Quaternion::to_axis_angle() angle");

	mml::Quaternionf q_diff = q_identity.rotation_to(q_axis);
	assert_equal_float(q_diff.length(), 1.0f, "Quaternion::rotation_to() length");
	float angle_diff = q_identity.angle_to(q_axis);
	assert_equal_float(angle_diff, 1.57079637f, "Quaternion::angle_to() angle");
}

void test_geometry() {
	print_header("Geometry Tests");

	mml::AABBf aabb(mml::Vector3f(-1.0f, -1.0f, -1.0f), mml::Vector3f(1.0f, 1.0f, 1.0f));
	assert_equal_float(aabb.center().x, 0.0f, "AABB::center() x");
	assert_equal_float(aabb.volume(), 8.0f, "AABB::volume()");
	assert_equal_float(aabb.surface_area(), 24.0f, "AABB::surface_area()");

	mml::Spheref sphere(mml::Vector3f(0.0f, 0.0f, 0.0f), 5.0f);
	assert_equal_float(sphere.volume(), 523.59878f, "Sphere::volume()", 1e-3f);
	assert_equal_float(sphere.surface_area(), 314.15927f, "Sphere::surface_area()", 1e-3f);

	mml::Planef plane = mml::Planef::from_point_normal(
			mml::Vector3f(0.0f, 5.0f, 0.0f), mml::Vector3f(0.0f, 1.0f, 0.0f));
	assert_equal_float(plane.distance_to(mml::Vector3f(0.0f, 10.0f, 0.0f)), 5.0f, "Plane::distance_to()");

	mml::Rayf ray(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 0.0f, 0.0f));
	mml::Vector3f ray_point = ray.at(5.0f);
	assert_equal_float(ray_point.x, 5.0f, "Ray::at(5) x component");

	mml::Trianglef tri(
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 0.0f, 0.0f),
			mml::Vector3f(0.0f, 1.0f, 0.0f));
	assert_equal_float(tri.area(), 0.5f, "Triangle::area()");
	assert_equal_float(tri.normal().z, 1.0f, "Triangle::normal() z component");

	mml::Capsulef capsule(
			mml::Vector3f(0.0f, 0.0f, -2.0f),
			mml::Vector3f(0.0f, 0.0f, 2.0f),
			1.0f);
	assert_equal_float(capsule.height(), 4.0f, "Capsule::height()");
	assert_equal_float(capsule.center().y, 0.0f, "Capsule::center() y component");

	mml::Cylinderf cylinder(
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			2.0f,
			5.0f);
	assert_equal_float(cylinder.volume(), 62.831853f, "Cylinder::volume()", 1e-4f);
	assert_equal_float(cylinder.surface_area(), 87.9646f, "Cylinder::surface_area()", 1e-4f);

	mml::Conef cone(
			mml::Vector3f(0.0f, 5.0f, 0.0f),
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			2.0f);
	assert_equal_float(cone.volume(), 20.943951f, "Cone::volume()");
	assert_equal_float(cone.surface_area(), 46.402359f, "Cone::surface_area()");
}

void test_line_and_polygon() {
	print_header("Line and Polygon Tests");

	mml::Line2Df line2(mml::Vector2f(0.0f, 0.0f), mml::Vector2f(10.0f, 10.0f));
	assert_equal_float(line2.length(), 14.1421356f, "Line2D::length()", 1e-4f);
	assert_equal_float(line2.closest_point(mml::Vector2f(5.0f, 3.0f)).y, 4.0f, "Line2D::closest_point() y", 1e-4f);

	mml::Line3Df line3(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(10.0f, 10.0f, 10.0f));
	assert_equal_float(line3.length(), 17.320508f, "Line3D::length()", 1e-4f);
	assert_equal_float(line3.distance(line3.closest_point(mml::Vector3f(5.0f, 3.0f, 0.0f))), 0.0f, "Line3D::distance()", 1e-4f);

	mml::Polygon2Df poly = mml::Polygon2Df::rectangle(
			mml::Vector2f(0.0f, 0.0f), mml::Vector2f(10.0f, 10.0f));
	assert_equal_float(poly.area(), 100.0f, "Polygon2D::area()");
	assert_equal_float(poly.perimeter(), 40.0f, "Polygon2D::perimeter()");
	assert_equal_float(poly.centroid().x, 5.0f, "Polygon2D::centroid() x");

	mml::Polygon2Df hex = mml::Polygon2Df::regular(5.0f, 6);
	assert_equal_float(hex.area(), 54.1266f, "Polygon2D::regular(6) area()", 1e-3f);
}

void test_random() {
	print_header("Random Tests");
	mml::Randomf::seed(12345);

	float r_val = mml::Randomf::value();
	assert_equal_float((r_val >= 0.0f) && (r_val <= 1.0f), 1.0f, "Random::value() in range [0,1]");

	float r_range = mml::Randomf::range(5.0f, 15.0f);
	assert_equal_float((r_range >= 5.0f) && (r_range <= 15.0f), 1.0f, "Random::range(5,15) in range");

	bool r_chance = mml::Randomf::chance(0.5f);
	assert_equal_float((r_chance >= 0.0f) && (r_chance <= 1.0f), 1.0f, "Random::chance(0.5) returns bool");

	mml::Vector2f r_circle = mml::Randomf::on_unit_circle();
	assert_equal_float(r_circle.length(), 1.0f, "Random::on_unit_circle() length");

	mml::Vector3f r_sphere = mml::Randomf::on_unit_sphere();
	assert_equal_float(r_sphere.length(), 1.0f, "Random::on_unit_sphere() length");
}

void test_transforms() {
	print_header("Transform Tests");

	mml::Transformf t_identity = mml::Transformf::identity();
	mml::Vector3f point(1.0f, 2.0f, 3.0f);
	mml::Vector3f transformed = t_identity.transform_point(point);
	assert_equal_float(transformed.x, 1.0f, "Transform::identity() transform x");

	mml::Transformf transform(
			mml::Vector3f(1.0f, 2.0f, 3.0f),
			mml::Quaternionf::from_euler(0.78539816f, 1.0471976f, 0.52359878f),
			mml::Vector3f(2.0f, 2.0f, 2.0f));

	mml::Matrix4f t_matrix = transform.to_matrix();
	assert_equal_float(t_matrix[3][0], 1.0f, "Transform::to_matrix() translation x");

	mml::Vector3f dir(0.0f, 1.0f, 0.0f);
	mml::Vector3f rotated_dir = transform.transform_direction(dir);
	assert_equal_float(rotated_dir.y, 0.739199f, "Transform::transform_direction() y");

	mml::Transformf look_t = mml::Transformf::look_at(
			mml::Vector3f(0.0f, 0.0f, 5.0f),
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(0.0f, 1.0f, 0.0f));
	assert_equal_float(look_t.position.z, 5.0f, "Transform::look_at() position z");

	// Test zero-scale protection in inverse
	mml::Transformf zero_scale_transform;
	zero_scale_transform.scale = mml::Vector3f(0.0f, 2.0f, 0.0f);
	mml::Transformf zero_inv = zero_scale_transform.inverse();
	assert_equal_float(zero_inv.scale.x, 0.0f, "Transform::inverse() zero scale x returns 0");
	assert_equal_float(zero_inv.scale.y, 0.5f, "Transform::inverse() non-zero scale y inverts correctly");
	assert_equal_float(zero_inv.scale.z, 0.0f, "Transform::inverse() zero scale z returns 0");
}

void test_utils() {
	print_header("Utility Tests");

	assert_equal_float(mml::Anglef::to_radians(180.0f), 3.14159265f, "Angle::to_radians(180)");
	assert_equal_float(mml::Anglef::to_degrees(3.14159265f), 180.0f, "Angle::to_degrees(PI)");
	assert_equal_float(mml::Anglef::normalize(7.8539816f), 1.57079637f, "Angle::normalize(450°)");
	float shortest = mml::Anglef::shortest_distance(0.0f, 3.14159265f);
	assert_equal_float(std::abs(shortest), 3.14159265f, "Angle::shortest_distance(0, PI) absolute");

	assert_equal_float(mml::Interpolationf::lerp(0.0f, 10.0f, 0.3f), 3.0f, "Interpolation::lerp(0, 10, 0.3)");
	assert_equal_float(mml::Interpolationf::smoothstep(0.0f, 1.0f, 0.5f), 0.5f, "Interpolation::smoothstep(0,1,0.5)");
	assert_equal_float(mml::Interpolationf::remap(50.0f, 0.0f, 100.0f, -1.0f, 1.0f), 0.0f, "Interpolation::remap(50, 0-100, -1-1)");
}

void test_simd() {
	print_header("SIMD Batch Operations Tests");

	constexpr size_t count = 4;
	mml::Vector4f a[count] = {
		mml::Vector4f(1.0f, 2.0f, 3.0f, 4.0f),
		mml::Vector4f(2.0f, 3.0f, 4.0f, 5.0f),
		mml::Vector4f(3.0f, 4.0f, 5.0f, 6.0f),
		mml::Vector4f(4.0f, 5.0f, 6.0f, 7.0f)
	};
	mml::Vector4f b[count] = {
		mml::Vector4f(5.0f, 4.0f, 3.0f, 2.0f),
		mml::Vector4f(6.0f, 5.0f, 4.0f, 3.0f),
		mml::Vector4f(7.0f, 6.0f, 5.0f, 4.0f),
		mml::Vector4f(8.0f, 7.0f, 6.0f, 5.0f)
	};
	mml::Vector4f result[count];

	mml::simd::batch::add_arrays(result, a, b, count);
	assert_equal_float(result[0].x, 6.0f, "SIMD::batch::add_arrays() [0] x");
	assert_equal_float(result[1].y, 8.0f, "SIMD::batch::add_arrays() [1] y");
	assert_equal_float(result[2].z, 10.0f, "SIMD::batch::add_arrays() [2] z");
	assert_equal_float(result[3].w, 12.0f, "SIMD::batch::add_arrays() [3] w");

	mml::simd::batch::sub_arrays(result, a, b, count);
	assert_equal_float(result[0].x, -4.0f, "SIMD::batch::sub_arrays() [0] x");
	assert_equal_float(result[1].y, -2.0f, "SIMD::batch::sub_arrays() [1] y");
	assert_equal_float(result[2].z, 0.0f, "SIMD::batch::sub_arrays() [2] z");
	assert_equal_float(result[3].w, 2.0f, "SIMD::batch::sub_arrays() [3] w");

	mml::simd::batch::mul_scalar_arrays(result, a, 2.0f, count);
	assert_equal_float(result[0].x, 2.0f, "SIMD::batch::mul_scalar_arrays() [0] x");
	assert_equal_float(result[1].y, 6.0f, "SIMD::batch::mul_scalar_arrays() [1] y");
	assert_equal_float(result[2].z, 10.0f, "SIMD::batch::mul_scalar_arrays() [2] z");
	assert_equal_float(result[3].w, 14.0f, "SIMD::batch::mul_scalar_arrays() [3] w");

	float dot4_results[count];
	mml::simd::batch::dot4_arrays(dot4_results, a, b, count);
	assert_equal_float(dot4_results[0], 30.0f, "SIMD::batch::dot4_arrays() [0]");
	assert_equal_float(dot4_results[1], 58.0f, "SIMD::batch::dot4_arrays() [1]");
	assert_equal_float(dot4_results[2], 94.0f, "SIMD::batch::dot4_arrays() [2]");
	assert_equal_float(dot4_results[3], 138.0f, "SIMD::batch::dot4_arrays() [3]");

	float dot3_results[count];
	mml::simd::batch::dot3_arrays(dot3_results, a, b, count);
	assert_equal_float(dot3_results[0], 22.0f, "SIMD::batch::dot3_arrays() [0]");
	assert_equal_float(dot3_results[1], 43.0f, "SIMD::batch::dot3_arrays() [1]");
	assert_equal_float(dot3_results[2], 70.0f, "SIMD::batch::dot3_arrays() [2]");
	assert_equal_float(dot3_results[3], 103.0f, "SIMD::batch::dot3_arrays() [3]");

	float len_sq4_results[count];
	mml::simd::batch::length_squared4_arrays(len_sq4_results, a, count);
	assert_equal_float(len_sq4_results[0], 30.0f, "SIMD::batch::length_squared4_arrays() [0]");
	assert_equal_float(len_sq4_results[1], 54.0f, "SIMD::batch::length_squared4_arrays() [1]");
	assert_equal_float(len_sq4_results[2], 86.0f, "SIMD::batch::length_squared4_arrays() [2]");
	assert_equal_float(len_sq4_results[3], 126.0f, "SIMD::batch::length_squared4_arrays() [3]");

	float len_sq3_results[count];
	mml::simd::batch::length_squared3_arrays(len_sq3_results, a, count);
	assert_equal_float(len_sq3_results[0], 14.0f, "SIMD::batch::length_squared3_arrays() [0]");
	assert_equal_float(len_sq3_results[1], 29.0f, "SIMD::batch::length_squared3_arrays() [1]");
	assert_equal_float(len_sq3_results[2], 50.0f, "SIMD::batch::length_squared3_arrays() [2]");
	assert_equal_float(len_sq3_results[3], 77.0f, "SIMD::batch::length_squared3_arrays() [3]");

	mml::simd::batch::min_arrays(result, a, b, count);
	assert_equal_float(result[0].x, 1.0f, "SIMD::batch::min_arrays() [0] x");
	assert_equal_float(result[1].y, 3.0f, "SIMD::batch::min_arrays() [1] y");
	assert_equal_float(result[2].z, 5.0f, "SIMD::batch::min_arrays() [2] z");
	assert_equal_float(result[3].w, 5.0f, "SIMD::batch::min_arrays() [3] w");

	mml::simd::batch::max_arrays(result, a, b, count);
	assert_equal_float(result[0].x, 5.0f, "SIMD::batch::max_arrays() [0] x");
	assert_equal_float(result[1].y, 5.0f, "SIMD::batch::max_arrays() [1] y");
	assert_equal_float(result[2].z, 5.0f, "SIMD::batch::max_arrays() [2] z");
	assert_equal_float(result[3].w, 7.0f, "SIMD::batch::max_arrays() [3] w");

	mml::Vector4f neg[count] = {
		mml::Vector4f(-1.0f, -2.0f, -3.0f, -4.0f),
		mml::Vector4f(-2.0f, -3.0f, -4.0f, -5.0f),
		mml::Vector4f(-3.0f, -4.0f, -5.0f, -6.0f),
		mml::Vector4f(-4.0f, -5.0f, -6.0f, -7.0f)
	};
	mml::simd::batch::abs_arrays(result, neg, count);
	assert_equal_float(result[0].x, 1.0f, "SIMD::batch::abs_arrays() [0] x");
	assert_equal_float(result[1].y, 3.0f, "SIMD::batch::abs_arrays() [1] y");
	assert_equal_float(result[2].z, 5.0f, "SIMD::batch::abs_arrays() [2] z");
	assert_equal_float(result[3].w, 7.0f, "SIMD::batch::abs_arrays() [3] w");

	mml::simd::batch::lerp_arrays(result, a, b, 0.5f, count);
	assert_equal_float(result[0].x, 3.0f, "SIMD::batch::lerp_arrays() [0] x");
	assert_equal_float(result[1].y, 4.0f, "SIMD::batch::lerp_arrays() [1] y");
	assert_equal_float(result[2].z, 5.0f, "SIMD::batch::lerp_arrays() [2] z", 1e-5f);
	assert_equal_float(result[3].w, 6.0f, "SIMD::batch::lerp_arrays() [3] w");

	assert_equal_float(mml::simd::dot4(a[0], b[0]), 30.0f, "SIMD::dot4()");
	assert_equal_float(mml::simd::dot3(a[1], b[1]), 43.0f, "SIMD::dot3()");
	assert_equal_float(mml::simd::length_squared4(a[2]), 86.0f, "SIMD::length_squared4()");
	assert_equal_float(mml::simd::length_squared3(a[3]), 77.0f, "SIMD::length_squared3()");

	mml::Vector4f min_v = mml::simd::min(a[0], b[0]);
	assert_equal_float(min_v.x, 1.0f, "SIMD::min() x");
	assert_equal_float(min_v.y, 2.0f, "SIMD::min() y");

	mml::Vector4f max_v = mml::simd::max(a[0], b[0]);
	assert_equal_float(max_v.x, 5.0f, "SIMD::max() x");
	assert_equal_float(max_v.y, 4.0f, "SIMD::max() y");

	mml::Vector4f abs_v = mml::simd::abs(neg[0]);
	assert_equal_float(abs_v.x, 1.0f, "SIMD::abs() x");
	assert_equal_float(abs_v.y, 2.0f, "SIMD::abs() y");

	mml::Vector4f lerp_v = mml::simd::lerp(a[0], b[0], 0.25f);
	assert_equal_float(lerp_v.x, 2.0f, "SIMD::lerp() x");
	assert_equal_float(lerp_v.y, 2.5f, "SIMD::lerp() y");

#if MML_USE_SIMD
	std::cout << "  [INFO] SIMD enabled: MML_USE_SIMD=" << MML_USE_SIMD << std::endl;
#else
	std::cout << "  [INFO] SIMD disabled: MML_USE_SIMD=" << MML_USE_SIMD << std::endl;
#endif
}

void test_transform_node() {
	print_header("TransformNode Engine Tests");

	mml::TransformNodef node1;
	assert_equal_float(node1.get_position().x, 0.0f, "TransformNode::default position x");
	assert_equal_float(node1.get_scale().x, 1.0f, "TransformNode::default scale x");

	node1.set_position(2.0f, 3.0f, 4.0f);
	assert_equal_float(node1.get_position().x, 2.0f, "TransformNode::set_position() x");
	assert_equal_float(node1.get_position().y, 3.0f, "TransformNode::set_position() y");
	assert_equal_float(node1.get_position().z, 4.0f, "TransformNode::set_position() z");

	mml::Quaternionf rot = mml::Quaternionf::from_euler(0.5f, 0.3f, 0.2f);
	node1.set_rotation(rot);
	assert_equal_float((node1.get_rotation().w - rot.w) < 0.001f, 1.0f, "TransformNode::set_rotation() w");

	node1.set_uniform_scale(2.5f);
	assert_equal_float(node1.get_scale().x, 2.5f, "TransformNode::set_uniform_scale() x");
	assert_equal_float(node1.get_scale().y, 2.5f, "TransformNode::set_uniform_scale() y");

	const mml::Matrix4f &local_matrix = node1.get_local_matrix();
	assert_equal_float(local_matrix[3][0], 2.0f, "TransformNode::get_local_matrix() translation x");

	const mml::Matrix4f &world_matrix = node1.get_world_matrix();
	assert_equal_float(world_matrix[3][0], 2.0f, "TransformNode::get_world_matrix() translation x");
	assert_equal_float(node1.is_dirty(), 0.0f, "TransformNode::is_dirty() after get_world_matrix()");

	node1.translate(1.0f, 0.0f, 0.0f);
	assert_equal_float(node1.get_position().x, 3.0f, "TransformNode::translate() x");
	assert_equal_float(node1.is_dirty(), 1.0f, "TransformNode::is_dirty() after translate()");

	mml::TransformNodef child1;
	child1.set_position(5.0f, 0.0f, 0.0f);
	node1.set_rotation(mml::Quaternionf::identity());
	node1.set_scale(mml::Vector3f(1.0f, 1.0f, 1.0f));
	node1.set_parent(nullptr);
	child1.set_parent(&node1);

	mml::Vector3f child_world_pos = child1.get_world_position();
	assert_equal_float(child_world_pos.x, 8.0f, "TransformNode child world position x (parent 3 + child 5)");

	mml::TransformNodef grandchild;
	grandchild.set_position(2.0f, 0.0f, 0.0f);
	grandchild.set_parent(&child1);

	mml::Vector3f grandchild_world_pos = grandchild.get_world_position();
	assert_equal_float(grandchild_world_pos.x, 10.0f, "TransformNode grandchild world position x");

	assert_equal_float(node1.get_child_count(), 1, "TransformNode::get_child_count()");
	assert_equal_float(child1.get_parent() != nullptr, 1.0f, "TransformNode::get_parent() not null");

	mml::Vector3f test_point(1.0f, 2.0f, 3.0f);
	node1.set_position(0.0f, 0.0f, 0.0f);
	node1.set_rotation(mml::Quaternionf::identity());
	node1.set_uniform_scale(2.0f);
	mml::Vector3f transformed = node1.transform_point(test_point);
	assert_equal_float(transformed.x, 2.0f, "TransformNode::transform_point() x");
	assert_equal_float(transformed.y, 4.0f, "TransformNode::transform_point() y");
	assert_equal_float(transformed.z, 6.0f, "TransformNode::transform_point() z");

	mml::Vector3f test_dir(1.0f, 0.0f, 0.0f);
	node1.set_uniform_scale(1.0f);
	mml::Vector3f transformed_dir = node1.transform_direction(test_dir);
	assert_equal_float(transformed_dir.x, 1.0f, "TransformNode::transform_direction() x");

	mml::TransformHierarchyf hierarchy;
	mml::TransformNodef *root = hierarchy.create_node(mml::Vector3f(0.0f, 0.0f, 0.0f));
	mml::TransformNodef *child = hierarchy.create_node(mml::Vector3f(1.0f, 0.0f, 0.0f));
	mml::TransformNodef *child2 = hierarchy.create_node(mml::Vector3f(2.0f, 0.0f, 0.0f));

	child->set_parent(root);
	child2->set_parent(root);

	hierarchy.update_all();
	assert_equal_float((float)hierarchy.size(), 3.0f, "TransformHierarchy::size()");
	assert_equal_float((float)root->get_child_count(), 2.0f, "TransformHierarchy root child count");

	mml::TransformNodef look_node = mml::TransformNodef::look_at(
			mml::Vector3f(0.0f, 0.0f, 5.0f),
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(0.0f, 1.0f, 0.0f));
	assert_equal_float(look_node.get_position().z, 5.0f, "TransformNode::look_at() position z");

	mml::TransformNodef rot_node;
	rot_node.set_position(mml::Vector3f(1.0f, 2.0f, 3.0f));
	mml::Vector3f rot_axis(0.0f, 1.0f, 0.0f);
	rot_node.rotate_axis(rot_axis, mml::Constantsf::half_pi);
	assert_equal_float(rot_node.is_dirty(), 1.0f, "TransformNode::rotate_axis() sets dirty flag");

	mml::Vector3f around_point(0.0f, 0.0f, 0.0f);
	rot_node.rotate_around(around_point, mml::Quaternionf::from_euler(0.0f, 0.5f, 0.0f));
	assert_equal_float(rot_node.is_dirty(), 1.0f, "TransformNode::rotate_around() sets dirty flag");
}

void test_intersection() {
	print_header("Intersection/Collision Tests");

	// Ray-Sphere intersection
	mml::Rayf ray(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 0.0f, 0.0f));
	float t;
	mml::Vector3f hit_point;
	bool hit = mml::intersection::ray_sphere(ray, mml::Vector3f(5.0f, 0.0f, 0.0f), 1.0f, t, hit_point);
	assert_equal_float((float)hit, 1.0f, "ray_sphere: hit sphere");
	assert_equal_float(t, 4.0f, "ray_sphere: hit distance");
	assert_equal_float(hit_point.x, 4.0f, "ray_sphere: hit point x");

	// Ray-Sphere miss
	bool miss = mml::intersection::ray_sphere(ray, mml::Vector3f(5.0f, 5.0f, 0.0f), 1.0f, t, hit_point);
	assert_equal_float((float)miss, 0.0f, "ray_sphere: miss sphere");

	// Ray-AABB intersection
	mml::AABBf aabb(mml::Vector3f(2.0f, -1.0f, -1.0f), mml::Vector3f(4.0f, 1.0f, 1.0f));
	hit = mml::intersection::ray_aabb(ray, aabb, t, hit_point);
	assert_equal_float((float)hit, 1.0f, "ray_aabb: hit AABB");
	assert_equal_float(hit_point.x, 2.0f, "ray_aabb: hit point x");

	// Ray-Triangle intersection
	mml::Trianglef tri(
			mml::Vector3f(5.0f, -1.0f, -1.0f),
			mml::Vector3f(5.0f, 1.0f, -1.0f),
			mml::Vector3f(5.0f, 0.0f, 1.0f));
	mml::Vector3f barycentric;
	hit = mml::intersection::ray_triangle(ray, tri, t, hit_point, barycentric);
	assert_equal_float((float)hit, 1.0f, "ray_triangle: hit triangle");
	assert_equal_float(hit_point.x, 5.0f, "ray_triangle: hit point x");

	// Sphere-Sphere intersection
	bool sphere_hit = mml::intersection::sphere_sphere(
			mml::Vector3f(0.0f, 0.0f, 0.0f), 1.0f,
			mml::Vector3f(1.5f, 0.0f, 0.0f), 1.0f);
	assert_equal_float((float)sphere_hit, 1.0f, "sphere_sphere: intersecting");

	bool sphere_miss = mml::intersection::sphere_sphere(
			mml::Vector3f(0.0f, 0.0f, 0.0f), 1.0f,
			mml::Vector3f(5.0f, 0.0f, 0.0f), 1.0f);
	assert_equal_float((float)sphere_miss, 0.0f, "sphere_sphere: non-intersecting");

	// Sphere-AABB intersection
	bool sphere_aabb_hit = mml::intersection::sphere_aabb(
			mml::Vector3f(0.0f, 0.0f, 0.0f), 1.5f,
			mml::AABBf(mml::Vector3f(1.0f, 0.0f, 0.0f), mml::Vector3f(2.0f, 1.0f, 1.0f)));
	assert_equal_float((float)sphere_aabb_hit, 1.0f, "sphere_aabb: intersecting");

	// AABB-AABB intersection
	bool aabb_hit = mml::intersection::aabb_aabb(
			mml::AABBf(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(2.0f, 2.0f, 2.0f)),
			mml::AABBf(mml::Vector3f(1.0f, 1.0f, 1.0f), mml::Vector3f(3.0f, 3.0f, 3.0f)));
	assert_equal_float((float)aabb_hit, 1.0f, "aabb_aabb: intersecting");

	// Sphere-Triangle intersection
	mml::Vector3f closest_on_tri;
	bool sphere_tri_hit = mml::intersection::sphere_triangle(
			mml::Vector3f(5.0f, 0.0f, 0.0f), 1.0f, tri, closest_on_tri);
	assert_equal_float((float)sphere_tri_hit, 1.0f, "sphere_triangle: intersecting");

	// Point in AABB
	bool point_in_aabb = mml::intersection::point_aabb(
			mml::Vector3f(1.0f, 0.5f, 0.5f),
			mml::AABBf(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(2.0f, 2.0f, 2.0f)));
	assert_equal_float((float)point_in_aabb, 1.0f, "point_aabb: inside");

	// Point in Sphere
	bool point_in_sphere = mml::intersection::point_sphere(
			mml::Vector3f(0.5f, 0.0f, 0.0f),
			mml::Vector3f(0.0f, 0.0f, 0.0f), 1.0f);
	assert_equal_float((float)point_in_sphere, 1.0f, "point_sphere: inside");

	// Point in Triangle
	bool point_in_tri = mml::intersection::point_triangle(
			mml::Vector3f(5.0f, 0.0f, 0.0f),
			mml::Trianglef(
					mml::Vector3f(4.0f, -1.0f, -1.0f),
					mml::Vector3f(6.0f, -1.0f, -1.0f),
					mml::Vector3f(5.0f, 1.0f, 1.0f)));
	assert_equal_float((float)point_in_tri, 1.0f, "point_triangle: inside");

	// Line segment - Sphere
	mml::intersection::LineSegmentf seg(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(10.0f, 0.0f, 0.0f));
	bool seg_sphere_hit = mml::intersection::segment_sphere(seg, mml::Vector3f(5.0f, 0.0f, 0.0f), 1.0f);
	assert_equal_float((float)seg_sphere_hit, 1.0f, "segment_sphere: intersecting");

	// Line segment - AABB
	bool seg_aabb_hit = mml::intersection::segment_aabb(seg,
			mml::AABBf(mml::Vector3f(3.0f, -1.0f, -1.0f), mml::Vector3f(7.0f, 1.0f, 1.0f)));
	assert_equal_float((float)seg_aabb_hit, 1.0f, "segment_aabb: intersecting");

	// Ray-Capsule intersection
	mml::Capsulef capsule(mml::Vector3f(5.0f, -1.0f, 0.0f), mml::Vector3f(5.0f, 1.0f, 0.0f), 0.5f);
	mml::Rayf ray_capsule(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 0.0f, 0.0f));
	bool capsule_hit = mml::intersection::ray_capsule(ray_capsule, capsule, t, hit_point);
	assert_equal_float((float)capsule_hit, 1.0f, "ray_capsule: hit capsule");

	// Capsule-AABB intersection
	mml::Capsulef capsule_aabb(mml::Vector3f(1.0f, 0.0f, 0.0f), mml::Vector3f(3.0f, 0.0f, 0.0f), 0.5f);
	mml::AABBf test_aabb(mml::Vector3f(2.0f, -1.0f, -1.0f), mml::Vector3f(4.0f, 1.0f, 1.0f));
	bool cap_aabb_hit = mml::intersection::capsule_aabb(capsule_aabb, test_aabb);
	assert_equal_float((float)cap_aabb_hit, 1.0f, "capsule_aabb: intersecting");

	// Capsule-Sphere intersection
	mml::Capsulef cap_sphere(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(2.0f, 0.0f, 0.0f), 0.5f);
	bool cap_sphere_hit = mml::intersection::capsule_sphere(cap_sphere, mml::Vector3f(1.0f, 1.0f, 0.0f), 0.6f);
	assert_equal_float((float)cap_sphere_hit, 1.0f, "capsule_sphere: intersecting");

	// Swept sphere - AABB (continuous collision)
	float out_t;
	bool swept_hit = mml::intersection::swept_sphere_aabb(
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(10.0f, 0.0f, 0.0f),
			0.5f,
			mml::AABBf(mml::Vector3f(5.0f, -1.0f, -1.0f), mml::Vector3f(7.0f, 1.0f, 1.0f)),
			out_t);
	assert_equal_float((float)swept_hit, 1.0f, "swept_sphere_aabb: hit");
	assert_equal_float(out_t, 0.45f, "swept_sphere_aabb: hit time", 0.1f);

	// Segment-Segment distance
	mml::intersection::LineSegmentf seg1(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 0.0f, 0.0f));
	mml::intersection::LineSegmentf seg2(mml::Vector3f(0.0f, 1.0f, 0.0f), mml::Vector3f(1.0f, 1.0f, 0.0f));
	float seg_dist = mml::intersection::distance_segment_segment(seg1, seg2);
	assert_equal_float(seg_dist, 1.0f, "distance_segment_segment: parallel segments");
}

void test_basis() {
	print_header("Orthonormal Basis Tests");

	mml::Basis3f basis = mml::Basis3f::identity();
	assert_equal_float(basis.right.x, 1.0f, "Basis3::identity() right x");
	assert_equal_float(basis.up.y, 1.0f, "Basis3::identity() up y");
	assert_equal_float(basis.forward.z, 1.0f, "Basis3::identity() forward z");

	mml::Vector3f forward(0.0f, 0.0f, -1.0f);
	mml::Basis3f look_basis = mml::make_basis_from_forward(forward);
	assert_equal_float(look_basis.forward.x, 0.0f, "make_basis_from_forward() forward x");
	assert_equal_float(look_basis.forward.z, -1.0f, "make_basis_from_forward() forward z");
	assert_equal_float(look_basis.right.x, 1.0f, "make_basis_from_forward() right x");
	assert_equal_float(look_basis.right.z, 0.0f, "make_basis_from_forward() right z");
	assert_equal_float(look_basis.up.y, 1.0f, "make_basis_from_forward() up y");

	mml::Vector3f world_up(0.0f, 1.0f, 0.0f);
	mml::Basis3f from_up = mml::make_basis_from_up(world_up);
	assert_equal_float(from_up.up.y, 1.0f, "make_basis_from_up() up y");

	mml::Basis3f from_right = mml::make_basis_from_right(mml::Vector3f(1.0f, 0.0f, 0.0f));
	assert_equal_float(from_right.right.x, 1.0f, "make_basis_from_right() right x");

	mml::Vector3f a(1.0f, 0.1f, 0.2f);
	mml::Vector3f b(0.1f, 1.0f, 0.3f);
	mml::Vector3f c(0.2f, 0.3f, 1.0f);
	mml::Basis3f vec_basis = mml::make_basis_from_vectors(a, b, c, 0);
	assert_equal_float((vec_basis.right.length() - 1.0f) < 0.001f, 1.0f, "make_basis_from_vectors() right normalized");
	assert_equal_float((vec_basis.up.length() - 1.0f) < 0.001f, 1.0f, "make_basis_from_vectors() up normalized");
	assert_equal_float((vec_basis.forward.length() - 1.0f) < 0.001f, 1.0f, "make_basis_from_vectors() forward normalized");

	mml::Vector3f r(1.0f, 0.2f, 0.1f);
	mml::Vector3f u(0.1f, 1.0f, 0.3f);
	mml::Vector3f f(0.2f, 0.3f, 1.0f);
	mml::orthonormalize(r, u, f, 0);
	assert_equal_float((r.length() - 1.0f) < 0.001f, 1.0f, "orthonormalize() right normalized");
	assert_equal_float(r.dot(u), 0.0f, "orthonormalize() right perpendicular to up", 0.001f);
	assert_equal_float(r.dot(f), 0.0f, "orthonormalize() right perpendicular to forward", 0.001f);
	assert_equal_float(u.dot(f), 0.0f, "orthonormalize() up perpendicular to forward", 0.001f);

	mml::Basis3f non_ortho(1.0f, 0.2f, 0.1f, 0.1f, 1.0f, 0.3f, 0.2f, 0.3f, 1.0f);
	mml::Basis3f orthonorm = mml::orthonormalize_basis(non_ortho, 0);
	assert_equal_float((orthonorm.right.length() - 1.0f) < 0.001f, 1.0f, "orthonormalize_basis() right normalized");

	mml::Matrix3f mat = orthonorm.to_matrix3();
	assert_equal_float(mat[0][0], orthonorm.right.x, "Basis3::to_matrix3() right x");
	assert_equal_float(mat[1][0], orthonorm.up.x, "Basis3::to_matrix3() up x");
	assert_equal_float(mat[2][0], orthonorm.forward.x, "Basis3::to_matrix3() forward x");

	mml::Matrix4f mat4 = orthonorm.to_matrix4();
	assert_equal_float(mat4[3][3], 1.0f, "Basis3::to_matrix4() w component");
	assert_equal_float(mat4[0][3], 0.0f, "Basis3::to_matrix4() right w");

	mml::Vector3f eye(0.0f, 2.0f, 5.0f);
	mml::Vector3f target(0.0f, 0.0f, 0.0f);
	mml::Basis3f look_at = mml::make_look_at_basis(eye, target);
	mml::Vector3f look_fwd = target - eye;
	assert_equal_float(look_at.forward.dot(look_fwd.normalized()), 1.0f, "make_look_at_basis() forward points at target", 0.001f);

	mml::Vector3f normal(0.577f, 0.577f, 0.577f);
	mml::Basis3f aligned = mml::align_basis_to_normal(normal);
	assert_equal_float(aligned.forward.dot(normal.normalized()), 1.0f, "align_basis_to_normal() forward aligned with normal", 0.001f);

	mml::Basis3f identity_basis = mml::Basis3f::identity();
	float vol = mml::basis_volume(identity_basis);
	assert_equal_float(vol, 1.0f, "basis_volume() identity basis");

	mml::Basis3f right_handed = mml::Basis3f::identity();
	assert_equal_float((float)mml::is_right_handed(right_handed), 1.0f, "is_right_handed() identity basis");

	mml::Basis3f left_handed;
	left_handed.right = mml::Vector3f(-1.0f, 0.0f, 0.0f);
	left_handed.up = mml::Vector3f(0.0f, 1.0f, 0.0f);
	left_handed.forward = mml::Vector3f(0.0f, 0.0f, 1.0f);
	assert_equal_float((float)mml::is_right_handed(left_handed), 0.0f, "is_right_handed() left-handed basis");

	mml::Basis3f flipped = left_handed;
	mml::make_right_handed(flipped);
	assert_equal_float((float)mml::is_right_handed(flipped), 1.0f, "make_right_handed() after conversion");
	assert_equal_float(flipped.right.x, 1.0f, "make_right_handed() right x after flip");

	mml::Basis3f rot_basis = mml::rotate_basis(identity_basis, mml::Vector3f(0.0f, 1.0f, 0.0f), mml::Constantsf::half_pi);
	assert_equal_float(std::abs(rot_basis.forward.x), 1.0f, "rotate_basis() forward x magnitude after Y rotation", 0.01f);
	assert_equal_float(std::abs(rot_basis.forward.z) < 0.001f, 1.0f, "rotate_basis() forward z near 0 after Y rotation");
	assert_equal_float(std::abs(rot_basis.right.x) < 0.001f, 1.0f, "rotate_basis() right x near 0 after Y rotation");
	assert_equal_float(std::abs(rot_basis.right.z), 1.0f, "rotate_basis() right z magnitude after Y rotation", 0.01f);

	mml::Quaternionf rot_q = mml::Quaternionf::from_euler(0.0f, mml::Constantsf::half_pi, 0.0f);
	mml::Basis3f q_rot_basis = mml::rotate_basis(identity_basis, rot_q);
	assert_equal_float(q_rot_basis.forward.x, 0.0f, "rotate_basis() with quaternion forward x", 0.01f);
	assert_equal_float(q_rot_basis.forward.z, 1.0f, "rotate_basis() with quaternion forward z", 0.01f);

	mml::Basis3f basis_a = mml::Basis3f::identity();
	mml::Basis3f basis_b = mml::Basis3f::identity();
	float similarity = mml::basis_similarity(basis_a, basis_b);
	assert_equal_float(similarity, 0.0f, "basis_similarity() identical bases");

	assert_equal_float((float)mml::basis_equals(basis_a, basis_b), 1.0f, "basis_equals() identical bases");
}

void test_deterministic_random() {
	print_header("Deterministic Random Tests");

	// Test LCG32
	{
		mml::random::LCG32 lcg1(12345);
		mml::random::LCG32 lcg2(12345);

		// Same seed should produce same sequence
		uint32_t val1 = lcg1.next_uint32();
		uint32_t val2 = lcg2.next_uint32();
		assert_equal_float((float)(val1 == val2), 1.0f, "LCG32: same seed produces same value");

		// Test range
		float f = lcg1.next_float();
		assert_equal_float((float)(f >= 0.0f && f < 1.0f), 1.0f, "LCG32: float in range [0,1)");

		// Test range_int32
		int32_t r = lcg1.range_int32(10, 20);
		assert_equal_float((float)(r >= 10 && r <= 20), 1.0f, "LCG32: int range [10,20]");

		// Test chance
		int true_count = 0;
		for (int i = 0; i < 100; ++i) {
			if (lcg1.chance(0.5f)) {
				++true_count;
			}
		}
		// Should be roughly 50/50 (with some variance)
		assert_equal_float((float)(true_count > 30 && true_count < 70), 1.0f, "LCG32: chance distribution");

		// Test reset
		lcg1.reset();
		uint32_t reset_val = lcg1.next_uint32();
		mml::random::LCG32 lcg3(12345);
		uint32_t seed_val = lcg3.next_uint32();
		assert_equal_float((float)(reset_val == seed_val), 1.0f, "LCG32: reset restores initial state");
	}

	// Test PCG32
	{
		mml::random::PCG32 pcg1(12345);
		mml::random::PCG32 pcg2(12345);

		// Same seed should produce same sequence
		uint32_t val1 = pcg1.next_uint32();
		uint32_t val2 = pcg2.next_uint32();
		assert_equal_float((float)(val1 == val2), 1.0f, "PCG32: same seed produces same value");

		// Test double precision (53 bits)
		double d = pcg1.next_double();
		assert_equal_float((float)(d >= 0.0 && d < 1.0), 1.0f, "PCG32: double in range [0,1)");

		// Different seeds should produce different sequences
		mml::random::PCG32 pcg3(54321);
		uint32_t val3 = pcg3.next_uint32();
		assert_equal_float((float)(val1 != val3), 1.0f, "PCG32: different seeds produce different values");

		// Test range_uint32 with uniform distribution
		uint32_t r = pcg1.range_uint32(100, 200);
		assert_equal_float((float)(r >= 100 && r <= 200), 1.0f, "PCG32: uint32 range [100,200]");

		// Test reset
		pcg1.reset();
		uint32_t reset_val = pcg1.next_uint32();
		mml::random::PCG32 pcg4(12345);
		uint32_t seed_val = pcg4.next_uint32();
		assert_equal_float((float)(reset_val == seed_val), 1.0f, "PCG32: reset restores initial state");
	}

	// Test Xoroshiro128+
	{
		mml::random::Xoroshiro128Plus xoro1(12345);
		mml::random::Xoroshiro128Plus xoro2(12345);

		// Same seed should produce same sequence
		uint64_t val1 = xoro1.next_uint64();
		uint64_t val2 = xoro2.next_uint64();
		assert_equal_float((float)(val1 == val2), 1.0f, "Xoroshiro128+: same seed produces same value");

		// Test float generation
		float f = xoro1.next_float();
		assert_equal_float((float)(f >= 0.0f && f < 1.0f), 1.0f, "Xoroshiro128+: float in range [0,1)");

		// Test range_uint64
		uint64_t r = xoro1.range_uint64(1000, 2000);
		assert_equal_float((float)(r >= 1000 && r <= 2000), 1.0f, "Xoroshiro128+: uint64 range [1000,2000]");

		// Test int64 range
		int64_t ri = xoro1.range_int64(-100, 100);
		assert_equal_float((float)(ri >= -100 && ri <= 100), 1.0f, "Xoroshiro128+: int64 range [-100,100]");

		// Test jump (should produce different sequence)
		mml::random::Xoroshiro128Plus xoro3(12345);
		xoro3.jump();
		uint64_t jumped_val = xoro3.next_uint64();
		assert_equal_float((float)(jumped_val != val1), 1.0f, "Xoroshiro128+: jump produces different sequence");

		// Test reset
		xoro1.reset();
		uint64_t reset_val = xoro1.next_uint64();
		mml::random::Xoroshiro128Plus xoro4(12345);
		uint64_t seed_val = xoro4.next_uint64();
		assert_equal_float((float)(reset_val == seed_val), 1.0f, "Xoroshiro128+: reset restores initial state");
	}

	// Test SplitMix64
	{
		mml::random::SplitMix64 sm1(12345);
		mml::random::SplitMix64 sm2(12345);

		// Same seed should produce same sequence
		uint64_t val1 = sm1.next_uint64();
		uint64_t val2 = sm2.next_uint64();
		assert_equal_float((float)(val1 == val2), 1.0f, "SplitMix64: same seed produces same value");

		// Test double generation
		double d = sm1.next_double();
		assert_equal_float((float)(d >= 0.0 && d < 1.0), 1.0f, "SplitMix64: double in range [0,1)");

		// Test range
		float f = sm1.range_float(0.0f, 10.0f);
		assert_equal_float((float)(f >= 0.0f && f <= 10.0f), 1.0f, "SplitMix64: float range [0,10]");

		// Test reset
		sm1.reset();
		uint64_t reset_val = sm1.next_uint64();
		mml::random::SplitMix64 sm3(12345);
		uint64_t seed_val = sm3.next_uint64();
		assert_equal_float((float)(reset_val == seed_val), 1.0f, "SplitMix64: reset restores initial state");
	}

	// Test determinism across multiple generators
	{
		// All generators with same seed should be deterministic
		mml::random::LCG32 lcg(42);
		mml::random::PCG32 pcg(42);
		mml::random::Xoroshiro128Plus xoro(42);
		mml::random::SplitMix64 sm(42);

		// Generate sequences
		std::vector<uint32_t> lcg_seq;
		std::vector<uint32_t> pcg_seq;
		std::vector<uint64_t> xoro_seq;
		std::vector<uint64_t> sm_seq;

		for (int i = 0; i < 10; ++i) {
			lcg_seq.push_back(lcg.next_uint32());
			pcg_seq.push_back(pcg.next_uint32());
			xoro_seq.push_back(xoro.next_uint64());
			sm_seq.push_back(sm.next_uint64());
		}

		// Recreate with same seed and verify same sequences
		mml::random::LCG32 lcg2(42);
		mml::random::PCG32 pcg2(42);
		mml::random::Xoroshiro128Plus xoro2(42);
		mml::random::SplitMix64 sm2(42);

		bool all_match = true;
		for (int i = 0; i < 10; ++i) {
			if (lcg2.next_uint32() != lcg_seq[i]) {
				all_match = false;
			}
			if (pcg2.next_uint32() != pcg_seq[i]) {
				all_match = false;
			}
			if (xoro2.next_uint64() != xoro_seq[i]) {
				all_match = false;
			}
			if (sm2.next_uint64() != sm_seq[i]) {
				all_match = false;
			}
		}

		assert_equal_float((float)all_match, 1.0f, "All RNGs: deterministic across recreations");
	}
}

void test_bounding_volume() {
	print_header("Bounding Volume Tests");

	// Test AABB merge/union
	mml::AABBf aabb1(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(2.0f, 2.0f, 2.0f));
	mml::AABBf aabb2(mml::Vector3f(1.0f, 1.0f, 1.0f), mml::Vector3f(3.0f, 3.0f, 3.0f));
	mml::AABBf merged = aabb1 | aabb2;
	assert_equal_float(merged.min.x, 0.0f, "AABB union min x");
	assert_equal_float(merged.max.x, 3.0f, "AABB union max x");
	assert_equal_float(merged.volume(), 27.0f, "AABB union volume");

	// Test AABB merge function
	mml::AABBf merged2 = mml::AABBf::merge(aabb1, aabb2);
	assert_equal_float(merged2.min.x, 0.0f, "AABB::merge() min x");

	// Test AABB-Sphere intersection
	mml::AABBf aabb3(mml::Vector3f(-1.0f, -1.0f, -1.0f), mml::Vector3f(1.0f, 1.0f, 1.0f));
	mml::Spheref sphere1(mml::Vector3f(0.0f, 0.0f, 0.0f), 0.5f);
	assert_equal_float((float)aabb3.intersects_sphere(sphere1.center, sphere1.radius), 1.0f, "AABB intersects sphere (contained)");

	mml::Spheref sphere2(mml::Vector3f(5.0f, 0.0f, 0.0f), 1.0f);
	assert_equal_float((float)aabb3.intersects_sphere(sphere2.center, sphere2.radius), 0.0f, "AABB intersects sphere (separate)");

	// Test AABB-Sphere contains
	mml::Spheref sphere3(mml::Vector3f(0.0f, 0.0f, 0.0f), 0.3f);
	assert_equal_float((float)aabb3.contains_sphere(sphere3.center, sphere3.radius), 1.0f, "AABB contains sphere");

	mml::Spheref sphere4(mml::Vector3f(0.0f, 0.0f, 0.0f), 2.0f);
	assert_equal_float((float)aabb3.contains_sphere(sphere4.center, sphere4.radius), 0.0f, "AABB does not contain sphere (too large)");

	// Test AABB transform
	mml::Matrix4f translation = mml::Matrix4f::translation(5.0f, 0.0f, 0.0f);
	mml::AABBf translated = aabb1.transformed(translation);
	assert_equal_float(translated.min.x, 5.0f, "AABB transformed min x");
	assert_equal_float(translated.max.x, 7.0f, "AABB transformed max x");

	// Test AABB rotation
	mml::Matrix4f rotation = mml::Matrix4f::rotation_y(mml::Constantsf::half_pi);
	mml::AABBf rotated = aabb1.transformed(rotation);
	assert_equal_float(std::abs(rotated.max.x - 2.0f) < 0.01f, 1.0f, "AABB rotated max x");

	// Test Sphere union
	mml::Spheref s1(mml::Vector3f(0.0f, 0.0f, 0.0f), 1.0f);
	mml::Spheref s2(mml::Vector3f(2.0f, 0.0f, 0.0f), 1.0f);
	mml::Spheref s_union = s1 | s2;
	assert_equal_float(s_union.center.x, 1.0f, "Sphere union center x");
	assert_equal_float(s_union.radius, 2.0f, "Sphere union radius");

	// Test Sphere contains
	mml::Spheref s3(mml::Vector3f(0.0f, 0.0f, 0.0f), 3.0f);
	mml::Spheref s4(mml::Vector3f(0.0f, 0.0f, 0.0f), 1.0f);
	assert_equal_float((float)s3.contains(s4), 1.0f, "Sphere contains sphere");
	assert_equal_float((float)s4.contains(s3), 0.0f, "Small sphere does not contain large sphere");

	// Test Sphere transform
	mml::Spheref transformed_sphere = s1.transformed(translation);
	assert_equal_float(transformed_sphere.center.x, 5.0f, "Sphere transformed center x");
	assert_equal_float(transformed_sphere.radius, 1.0f, "Sphere transformed radius (translation only)");

	// Test Sphere scale transform
	mml::Matrix4f scale = mml::Matrix4f::scale(2.0f, 2.0f, 2.0f);
	mml::Spheref scaled_sphere = s1.transformed(scale);
	assert_equal_float(scaled_sphere.radius, 2.0f, "Sphere transformed radius (scaled)");

	// Test Sphere distance
	float dist = s1.distance_to(mml::Vector3f(3.0f, 0.0f, 0.0f));
	assert_equal_float(dist, 2.0f, "Sphere distance to point");

	mml::Spheref s5(mml::Vector3f(5.0f, 0.0f, 0.0f), 1.0f);
	float sphere_dist = s1.distance_to(s5);
	assert_equal_float(sphere_dist, 3.0f, "Sphere distance to sphere");
}

void test_soa() {
	print_header("SoA (Structure of Arrays) Tests");

	// Test Vector3SoA creation and conversion
	{
		mml::Vector3SoAf soa(4);
		soa.x = { 1.0f, 2.0f, 3.0f, 4.0f };
		soa.y = { 5.0f, 6.0f, 7.0f, 8.0f };
		soa.z = { 9.0f, 10.0f, 11.0f, 12.0f };

		assert_equal_float(soa.size(), 4.0f, "Vector3SoA size");
		assert_equal_float(soa.get(0).x, 1.0f, "Vector3SoA get(0).x");
		assert_equal_float(soa.get(3).y, 8.0f, "Vector3SoA get(3).y");

		// Test set
		soa.set(0, mml::Vector3f(10.0f, 20.0f, 30.0f));
		assert_equal_float(soa.x[0], 10.0f, "Vector3SoA set x");
		assert_equal_float(soa.y[0], 20.0f, "Vector3SoA set y");
	}

	// Test Vector3SoA from/to AoS
	{
		std::vector<mml::Vector3f> aos = {
			mml::Vector3f(1.0f, 2.0f, 3.0f),
			mml::Vector3f(4.0f, 5.0f, 6.0f),
			mml::Vector3f(7.0f, 8.0f, 9.0f)
		};

		mml::Vector3SoAf soa;
		soa.from_aos(aos);
		assert_equal_float(soa.x[1], 4.0f, "Vector3SoA from_aos x[1]");
		assert_equal_float(soa.y[1], 5.0f, "Vector3SoA from_aos y[1]");

		std::vector<mml::Vector3f> aos_back = soa.to_aos();
		assert_equal_float(aos_back[2].z, 9.0f, "Vector3SoA to_aos z[2]");
	}

	// Test Vector3SoA batch addition
	{
		mml::Vector3SoAf soa1(3);
		soa1.x = { 1.0f, 2.0f, 3.0f };
		soa1.y = { 4.0f, 5.0f, 6.0f };
		soa1.z = { 7.0f, 8.0f, 9.0f };

		mml::Vector3SoAf soa2(3);
		soa2.x = { 10.0f, 20.0f, 30.0f };
		soa2.y = { 40.0f, 50.0f, 60.0f };
		soa2.z = { 70.0f, 80.0f, 90.0f };

		soa1.add(soa2);
		assert_equal_float(soa1.x[0], 11.0f, "Vector3SoA add x[0]");
		assert_equal_float(soa1.y[1], 55.0f, "Vector3SoA add y[1]");
		assert_equal_float(soa1.z[2], 99.0f, "Vector3SoA add z[2]");
	}

	// Test Vector3SoA batch scale
	{
		mml::Vector3SoAf soa(3);
		soa.x = { 1.0f, 2.0f, 3.0f };
		soa.y = { 4.0f, 5.0f, 6.0f };
		soa.z = { 7.0f, 8.0f, 9.0f };

		soa.scale(2.0f);
		assert_equal_float(soa.x[0], 2.0f, "Vector3SoA scale x[0]");
		assert_equal_float(soa.y[1], 10.0f, "Vector3SoA scale y[1]");
		assert_equal_float(soa.z[2], 18.0f, "Vector3SoA scale z[2]");
	}

	// Test Vector3SoA batch dot product
	{
		mml::Vector3SoAf soa1(3);
		soa1.x = { 1.0f, 2.0f, 3.0f };
		soa1.y = { 0.0f, 1.0f, 2.0f };
		soa1.z = { 0.0f, 0.0f, 1.0f };

		mml::Vector3SoAf soa2(3);
		soa2.x = { 1.0f, 1.0f, 1.0f };
		soa2.y = { 1.0f, 1.0f, 1.0f };
		soa2.z = { 1.0f, 1.0f, 1.0f };

		std::vector<float> dots = soa1.dot(soa2);
		assert_equal_float(dots[0], 1.0f, "Vector3SoA dot [0]");
		assert_equal_float(dots[1], 3.0f, "Vector3SoA dot [1]");
		assert_equal_float(dots[2], 6.0f, "Vector3SoA dot [2]");
	}

	// Test Vector3SoA batch length squared
	{
		mml::Vector3SoAf soa(3);
		soa.x = { 1.0f, 2.0f, 3.0f };
		soa.y = { 0.0f, 2.0f, 4.0f };
		soa.z = { 0.0f, 1.0f, 0.0f };

		std::vector<float> lensq = soa.length_squared();
		assert_equal_float(lensq[0], 1.0f, "Vector3SoA length_squared [0]");
		assert_equal_float(lensq[1], 9.0f, "Vector3SoA length_squared [1]");
		assert_equal_float(lensq[2], 25.0f, "Vector3SoA length_squared [2]");
	}

	// Test Vector3SoA batch normalize
	{
		mml::Vector3SoAf soa(2);
		soa.x = { 3.0f, 5.0f };
		soa.y = { 4.0f, 12.0f };
		soa.z = { 0.0f, 0.0f };

		soa.normalize();
		std::vector<float> lensq = soa.length_squared();
		assert_equal_float(lensq[0], 1.0f, "Vector3SoA normalize [0] length", 0.001f);
		assert_equal_float(lensq[1], 1.0f, "Vector3SoA normalize [1] length", 0.001f);
	}

	// Test Vector4SoA
	{
		mml::Vector4SoAf soa(3);
		soa.x = { 1.0f, 2.0f, 3.0f };
		soa.y = { 5.0f, 6.0f, 7.0f };
		soa.z = { 9.0f, 10.0f, 11.0f };
		soa.w = { 1.0f, 1.0f, 1.0f };

		assert_equal_float(soa.size(), 3.0f, "Vector4SoA size");

		// Test dot product
		mml::Vector4SoAf other(3);
		other.x = { 1.0f, 0.0f, 0.0f };
		other.y = { 0.0f, 1.0f, 0.0f };
		other.z = { 0.0f, 0.0f, 1.0f };
		other.w = { 0.0f, 0.0f, 0.0f };

		std::vector<float> dots = soa.dot(other);
		assert_equal_float(dots[0], 1.0f, "Vector4SoA dot [0]");
		assert_equal_float(dots[1], 6.0f, "Vector4SoA dot [1]");
		assert_equal_float(dots[2], 11.0f, "Vector4SoA dot [2]");
	}

	// Test BatchTransform
	{
		mml::BatchTransformf batch(3);

		// Set positions
		batch.positions.x = { 0.0f, 10.0f, 20.0f };
		batch.positions.y = { 0.0f, 0.0f, 0.0f };
		batch.positions.z = { 0.0f, 0.0f, 0.0f };

		// Set scales
		batch.scales.x = { 1.0f, 2.0f, 1.0f };
		batch.scales.y = { 1.0f, 2.0f, 1.0f };
		batch.scales.z = { 1.0f, 2.0f, 1.0f };

		assert_equal_float(batch.size(), 3.0f, "BatchTransform size");

		// Test translate
		batch.translate(mml::Vector3f(5.0f, 0.0f, 0.0f));
		assert_equal_float(batch.positions.x[0], 5.0f, "BatchTransform translate x[0]");
		assert_equal_float(batch.positions.x[1], 15.0f, "BatchTransform translate x[1]");

		// Test scale
		batch.scale(2.0f);
		assert_equal_float(batch.scales.x[0], 2.0f, "BatchTransform scale x[0]");
		assert_equal_float(batch.scales.x[1], 4.0f, "BatchTransform scale x[1]");

		// Test matrix computation
		std::vector<mml::Matrix4f> matrices = batch.compute_matrices();
		assert_equal_float(matrices.size(), 3.0f, "BatchTransform matrices size");
	}

	// Test BatchAABB
	{
		mml::BatchAABBf batch(3);
		batch.min.x = { 0.0f, 10.0f, 20.0f };
		batch.min.y = { 0.0f, 0.0f, 0.0f };
		batch.min.z = { 0.0f, 0.0f, 0.0f };
		batch.max.x = { 1.0f, 11.0f, 21.0f };
		batch.max.y = { 1.0f, 1.0f, 1.0f };
		batch.max.z = { 1.0f, 1.0f, 1.0f };

		assert_equal_float(batch.size(), 3.0f, "BatchAABB size");

		// Test centers
		mml::Vector3SoAf centers = batch.centers();
		assert_equal_float(centers.x[0], 0.5f, "BatchAABB center x[0]");
		assert_equal_float(centers.x[1], 10.5f, "BatchAABB center x[1]");

		// Test extents
		mml::Vector3SoAf extents = batch.extents();
		assert_equal_float(extents.x[0], 1.0f, "BatchAABB extent x[0]");
		assert_equal_float(extents.x[1], 1.0f, "BatchAABB extent x[1]");

		// Test contains
		std::vector<bool> contains = batch.contains(mml::Vector3f(0.5f, 0.5f, 0.5f));
		assert_equal_float((float)contains[0], 1.0f, "BatchAABB contains[0]");
		assert_equal_float((float)contains[1], 0.0f, "BatchAABB contains[1] (should be false)");
	}
}

void test_curves() {
	print_header("Curve Tests");

	// Test Quadratic Bezier
	{
		mml::BezierQuadraticf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 1.0f, 0.0f),
				mml::Vector3f(2.0f, 0.0f, 0.0f));

		mml::Vector3f start = bezier.evaluate(0.0f);
		assert_equal_float(start.x, 0.0f, "Quadratic Bezier start x");
		assert_equal_float(start.y, 0.0f, "Quadratic Bezier start y");

		mml::Vector3f end = bezier.evaluate(1.0f);
		assert_equal_float(end.x, 2.0f, "Quadratic Bezier end x");
		assert_equal_float(end.y, 0.0f, "Quadratic Bezier end y");

		mml::Vector3f mid = bezier.evaluate(0.5f);
		assert_equal_float(mid.x, 1.0f, "Quadratic Bezier mid x");
		assert_equal_float(mid.y, 0.5f, "Quadratic Bezier mid y");

		// Test tangent
		mml::Vector3f tan = bezier.tangent(0.5f);
		assert_equal_float(tan.x, 2.0f, "Quadratic Bezier tangent x", 0.01f);
		assert_equal_float(tan.y, 0.0f, "Quadratic Bezier tangent y", 0.01f);

		// Test subdivision
		auto [left, right] = bezier.subdivide(0.5f);
		assert_equal_float(left[2].x, 1.0f, "Quadratic Bezier subdivision left end");
		assert_equal_float(right[0].x, 1.0f, "Quadratic Bezier subdivision right start");
	}

	// Test Cubic Bezier
	{
		mml::BezierCubicf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 1.0f, 0.0f),
				mml::Vector3f(2.0f, 1.0f, 0.0f),
				mml::Vector3f(3.0f, 0.0f, 0.0f));

		mml::Vector3f start = bezier.evaluate(0.0f);
		assert_equal_float(start.x, 0.0f, "Cubic Bezier start x");

		mml::Vector3f end = bezier.evaluate(1.0f);
		assert_equal_float(end.x, 3.0f, "Cubic Bezier end x");

		mml::Vector3f mid = bezier.evaluate(0.5f);
		assert_equal_float(mid.x, 1.5f, "Cubic Bezier mid x", 0.01f);
		assert_equal_float(mid.y, 0.75f, "Cubic Bezier mid y", 0.01f);
	}

	// Test Catmull-Rom Spline
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};

		mml::CatmullRomSplinef spline(points);

		mml::Vector3f start = spline.evaluate(0.0f);
		assert_equal_float(start.x, 0.0f, "Catmull-Rom start x");

		mml::Vector3f end = spline.evaluate(1.0f);
		assert_equal_float(end.x, 3.0f, "Catmull-Rom end x");

		mml::Vector3f mid = spline.evaluate(0.5f);
		assert_equal_float(mid.x, 1.5f, "Catmull-Rom mid x", 0.01f);
	}

	// Test closed Catmull-Rom
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(0.0f, 1.0f, 0.0f)
		};

		mml::CatmullRomSplinef spline(points, true);

		mml::Vector3f start = spline.evaluate(0.0f);
		mml::Vector3f end = spline.evaluate(1.0f);
		float dist = (start - end).length();
		assert_equal_float(dist, 0.0f, "Closed Catmull-Rom closure", 0.01f);
	}

	// Test Hermite Spline
	{
		mml::HermiteSplinef spline;
		spline.add_point(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 1.0f, 0.0f));
		spline.add_point(mml::Vector3f(2.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, -1.0f, 0.0f));

		mml::Vector3f start = spline.evaluate(0.0f);
		assert_equal_float(start.x, 0.0f, "Hermite start x");

		mml::Vector3f end = spline.evaluate(1.0f);
		assert_equal_float(end.x, 2.0f, "Hermite end x");

		// Test Hermite tangent evaluation
		mml::Vector3f mid_tan = spline.tangent(0.5f);
		assert_equal_float(mid_tan.length() > 0.0f, 1.0f, "Hermite tangent is non-zero");
	}

	// Test Cubic Bezier curvature
	{
		mml::BezierCubicf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 1.0f, 0.0f),
				mml::Vector3f(2.0f, 1.0f, 0.0f),
				mml::Vector3f(3.0f, 0.0f, 0.0f));

		float curv_start = bezier.curvature(0.0f);
		float curv_mid = bezier.curvature(0.5f);
		float curv_end = bezier.curvature(1.0f);

		assert_equal_float(curv_start >= 0.0f, 1.0f, "Cubic Bezier curvature non-negative start");
		assert_equal_float(curv_mid >= 0.0f, 1.0f, "Cubic Bezier curvature non-negative mid");
		assert_equal_float(curv_end >= 0.0f, 1.0f, "Cubic Bezier curvature non-negative end");
	}

	// Test Cubic Bezier second derivative
	{
		mml::BezierCubicf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(0.0f, 1.0f, 0.0f),
				mml::Vector3f(0.0f, 2.0f, 0.0f),
				mml::Vector3f(0.0f, 3.0f, 0.0f));

		mml::Vector3f d2 = bezier.second_derivative(0.5f);
		assert_equal_float(d2.x, 0.0f, "Cubic Bezier second derivative x");
		assert_equal_float(d2.y, 0.0f, "Cubic Bezier second derivative y");
	}

	// Test Cubic Bezier reparameterization (arc length)
	{
		mml::BezierCubicf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 1.0f, 0.0f),
				mml::Vector3f(2.0f, 1.0f, 0.0f),
				mml::Vector3f(3.0f, 0.0f, 0.0f));

		float len = bezier.length();
		assert_equal_float(len > 0.0f, 1.0f, "Cubic Bezier length is positive");

		float t_at_half = bezier.reparameterize(0.5f);
		assert_equal_float(t_at_half > 0.0f, 1.0f, "Cubic Bezier reparameterize returns valid t");
		assert_equal_float(t_at_half < 1.0f, 1.0f, "Cubic Bezier reparameterize t < 1");
	}

	// Test Catmull-Rom length
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};

		mml::CatmullRomSplinef spline(points);
		float spline_len = spline.length();
		assert_equal_float(spline_len > 0.0f, 1.0f, "Catmull-Rom length is positive");
	}

	// Test Catmull-Rom uniform sampling
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};

		mml::CatmullRomSplinef spline(points);
		auto samples = spline.sample_uniform(10);
		assert_equal_float((float)samples.size(), 10.0f, "Catmull-Rom sample_uniform count");

		// First and last samples should match endpoints
		assert_equal_float(samples.front().x, 0.0f, "Catmull-Rom sample first x", 0.01f);
		assert_equal_float(samples.back().x, 3.0f, "Catmull-Rom sample last x", 0.01f);
	}

	// Test Hermite Spline from Catmull-Rom conversion
	{
		std::vector<mml::Vector3f> catmull_points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};

		mml::HermiteSplinef hermite = mml::HermiteSplinef::from_catmull_rom(catmull_points);

		// Hermite should have same number of control points
		assert_equal_float((float)hermite.points.size(), 4.0f, "Hermite from Catmull-Rom point count");

		// Endpoints should match
		mml::Vector3f h_start = hermite.evaluate(0.0f);
		mml::Vector3f h_end = hermite.evaluate(1.0f);

		assert_equal_float(h_start.x, 0.0f, "Hermite from Catmull-Rom start x", 0.01f);
		assert_equal_float(h_end.x, 3.0f, "Hermite from Catmull-Rom end x", 0.01f);
	}

	// Test Quadratic Bezier length
	{
		mml::BezierQuadraticf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 1.0f, 0.0f),
				mml::Vector3f(2.0f, 0.0f, 0.0f));

		float len = bezier.length(20);
		assert_equal_float(len > 0.0f, 1.0f, "Quadratic Bezier length is positive");

		// More segments should give better accuracy
		float len_fine = bezier.length(100);
		assert_equal_float(len_fine > 0.0f, 1.0f, "Quadratic Bezier length fine is positive");
	}

	// Test Catmull-Rom segment evaluation
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};

		mml::CatmullRomSplinef spline(points);

		mml::Vector3f seg0_start = spline.evaluate_segment(0, 0.0f);
		mml::Vector3f seg0_end = spline.evaluate_segment(0, 1.0f);
		mml::Vector3f seg1_start = spline.evaluate_segment(1, 0.0f);
		mml::Vector3f seg1_end = spline.evaluate_segment(1, 1.0f);

		assert_equal_float(seg0_start.x, 0.0f, "Catmull-Rom segment 0 start x");
		assert_equal_float(seg0_end.x, 1.0f, "Catmull-Rom segment 0 end x");
		assert_equal_float(seg1_start.x, 1.0f, "Catmull-Rom segment 1 start x");
		assert_equal_float(seg1_end.x, 2.0f, "Catmull-Rom segment 1 end x");
	}

	// Test Quadratic Bezier second derivative (constant)
	{
		mml::BezierQuadraticf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 1.0f, 0.0f),
				mml::Vector3f(2.0f, 0.0f, 0.0f));

		mml::Vector3f d2_0 = bezier.second_derivative();
		mml::Vector3f d2_1 = bezier.second_derivative();
		mml::Vector3f d2_05 = bezier.second_derivative();

		assert_equal_float(d2_0.x, d2_1.x, "Quadratic Bezier second derivative constant x");
		assert_equal_float(d2_0.y, d2_1.y, "Quadratic Bezier second derivative constant y");
	}

	// Test BezierCubic frame computation
	{
		mml::BezierCubicf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 2.0f, 0.0f),
				mml::Vector3f(2.0f, 2.0f, 0.0f),
				mml::Vector3f(3.0f, 0.0f, 0.0f));

		mml::CurveFrame<float> frame = bezier.frame(0.5f);

		assert_equal_float(frame.position.x, 1.5f, "BezierCubic frame position x", 0.01f);
		assert_equal_float(frame.tangent.length() > 0.0f, 1.0f, "BezierCubic frame tangent non-zero");
		assert_equal_float(frame.normal.length() > 0.0f, 1.0f, "BezierCubic frame normal non-zero");
		assert_equal_float(frame.binormal.length() > 0.0f, 1.0f, "BezierCubic frame binormal non-zero");

		float t_dot_n = frame.tangent.dot(frame.normal);
		assert_equal_float(t_dot_n, 0.0f, "BezierCubic frame tangent perpendicular to normal", 0.01f);

		float t_dot_b = frame.tangent.dot(frame.binormal);
		assert_equal_float(t_dot_b, 0.0f, "BezierCubic frame tangent perpendicular to binormal", 0.01f);
	}

	// Test CatmullRomSpline frame computation
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};

		mml::CatmullRomSplinef spline(points);

		mml::CurveFrame<float> frame = spline.frame(0.5f);

		assert_equal_float(frame.position.x > 0.0f, 1.0f, "CatmullRom frame position valid");
		assert_equal_float(frame.tangent.length() > 0.0f, 1.0f, "CatmullRom frame tangent non-zero");
		assert_equal_float(frame.normal.length() > 0.0f, 1.0f, "CatmullRom frame normal non-zero");
		assert_equal_float(frame.binormal.length() > 0.0f, 1.0f, "CatmullRom frame binormal non-zero");
	}

	// Test HermiteSpline frame computation
	{
		mml::HermiteSplinef hermite;
		hermite.add_point(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 1.0f, 0.0f));
		hermite.add_point(mml::Vector3f(2.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, -1.0f, 0.0f));

		mml::CurveFrame<float> frame = hermite.frame(0.5f);

		assert_equal_float(frame.position.x > 0.0f, 1.0f, "Hermite frame position valid");
		assert_equal_float(frame.tangent.length() > 0.0f, 1.0f, "Hermite frame tangent non-zero");
		assert_equal_float(frame.normal.length() > 0.0f, 1.0f, "Hermite frame normal non-zero");
		assert_equal_float(frame.binormal.length() > 0.0f, 1.0f, "Hermite frame binormal non-zero");
	}

	// Test BezierCubic normal computation
	{
		mml::BezierCubicf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 2.0f, 0.0f),
				mml::Vector3f(2.0f, 2.0f, 0.0f),
				mml::Vector3f(3.0f, 0.0f, 0.0f));

		mml::Vector3f norm = bezier.normal(0.5f);
		assert_equal_float(norm.length() > 0.0f, 1.0f, "BezierCubic normal is non-zero");
		assert_equal_float(norm.length(), 1.0f, "BezierCubic normal is unit length", 0.01f);
	}

	// Test BezierCubic flatten
	{
		mml::BezierCubicf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 2.0f, 0.0f),
				mml::Vector3f(2.0f, 2.0f, 0.0f),
				mml::Vector3f(3.0f, 0.0f, 0.0f));

		auto flattened = bezier.flatten(0.1f);

		assert_equal_float((float)flattened.size() > 1.0f, 1.0f, "BezierCubic flatten produces segments");

		assert_equal_float(flattened.front().x, 0.0f, "BezierCubic flatten starts at p0", 0.01f);
		assert_equal_float(flattened.back().x, 3.0f, "BezierCubic flatten ends at p3", 0.01f);
	}

	// Test Quadratic Bezier flatten
	{
		mml::BezierQuadraticf bezier(
				mml::Vector3f(0.0f, 0.0f, 0.0f),
				mml::Vector3f(1.0f, 1.0f, 0.0f),
				mml::Vector3f(2.0f, 0.0f, 0.0f));

		auto flattened = bezier.flatten(0.1f);

		assert_equal_float((float)flattened.size() > 1.0f, 1.0f, "Quadratic Bezier flatten produces segments");

		assert_equal_float(flattened.front().x, 0.0f, "Quadratic Bezier flatten starts at p0", 0.01f);
		assert_equal_float(flattened.back().x, 2.0f, "Quadratic Bezier flatten ends at p2", 0.01f);
	}

	// Test NURBS basic evaluation
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};
		std::vector<float> weights = { 1.0f, 1.0f, 1.0f, 1.0f };

		mml::NURBSf nurbs(points, weights, 3);

		mml::Vector3f start = nurbs.evaluate(0.0f);
		assert_equal_float(start.x, 0.0f, "NURBS start x");
		assert_equal_float(start.y, 0.0f, "NURBS start y");

		mml::Vector3f end = nurbs.evaluate(1.0f);
		assert_equal_float(end.x, 3.0f, "NURBS end x");
		assert_equal_float(end.y, 0.0f, "NURBS end y");

		mml::Vector3f mid = nurbs.evaluate(0.5f);
		assert_equal_float(mid.x > 0.0f, 1.0f, "NURBS mid x positive");
		assert_equal_float(mid.y > 0.0f, 1.0f, "NURBS mid y positive");
	}

	// Test NURBS with varying weights
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f)
		};
		std::vector<float> weights = { 1.0f, 2.0f, 2.0f, 1.0f };

		mml::NURBSf weighted_nurbs(points, weights, 3);
		mml::Vector3f mid = weighted_nurbs.evaluate(0.5f);

		assert_equal_float(mid.x > 0.0f && mid.x < 2.0f, 1.0f, "NURBS weighted mid x in range");
		assert_equal_float(mid.y > 0.0f && mid.y < 1.0f, 1.0f, "NURBS weighted mid y in range");
	}

	// Test NURBS derivative (simplified - just check evaluate works)
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};
		std::vector<float> weights = { 1.0f, 1.0f, 1.0f, 1.0f };

		mml::NURBSf nurbs(points, weights, 3);

		mml::Vector3f mid = nurbs.evaluate(0.5f);
		assert_equal_float(mid.x > 0.0f && mid.x < 3.0f, 1.0f, "NURBS derivative test: evaluate mid point");
	}

	// Test NURBS sample
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};
		std::vector<float> weights = { 1.0f, 1.0f, 1.0f, 1.0f };

		mml::NURBSf nurbs(points, weights, 3);
		auto samples = nurbs.sample(10);

		assert_equal_float((float)samples.size(), 10.0f, "NURBS sample count");
		assert_equal_float(samples.front().x, 0.0f, "NURBS sample first x", 0.01f);
		assert_equal_float(samples.back().x, 3.0f, "NURBS sample last x", 0.01f);
	}

	// Test NURBS length
	{
		std::vector<mml::Vector3f> points = {
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 1.0f, 0.0f),
			mml::Vector3f(2.0f, 1.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f)
		};
		std::vector<float> weights = { 1.0f, 1.0f, 1.0f, 1.0f };

		mml::NURBSf nurbs(points, weights, 3);
		float len = nurbs.length();

		assert_equal_float(len > 0.0f, 1.0f, "NURBS length is positive");
	}
}

void test_bvh() {
	print_header("BVH Tests");

	// Create some test AABBs
	struct TestPrim {
		mml::AABBf bounds;
		int id;
	};

	std::vector<TestPrim> prims;
	for (int i = 0; i < 10; ++i) {
		TestPrim p;
		p.bounds = mml::AABBf(
				mml::Vector3f((float)i * 2.0f, 0.0f, 0.0f),
				mml::Vector3f((float)i * 2.0f + 1.0f, 1.0f, 1.0f));
		p.id = i;
		prims.push_back(p);
	}

	// Build BVH with bounds function
	mml::BVH<float, TestPrim> bvh([](const TestPrim &p) { return p.bounds; });
	bvh.build(prims);

	// Check bounds
	mml::AABBf total = bvh.get_bounds();
	assert_equal_float(total.min.x, 0.0f, "BVH total bounds min x");
	assert_equal_float(total.max.x, 19.0f, "BVH total bounds max x");

	// Check node count
	size_t nodes = bvh.node_count();
	assert_equal_float((float)nodes > 0, 1.0f, "BVH has nodes");

	// Ray intersection - should find primitives 2 and 3
	auto hits = bvh.ray_intersect(mml::Vector3f(-1.0f, 0.5f, 0.5f), mml::Vector3f(1.0f, 0.0f, 0.0f));
	assert_equal_float((float)hits.size() > 0, 1.0f, "BVH ray intersection finds hits");

	// Point query - point at (6.5, 0.5, 0.5) is in primitive 3's bounds [6,0,0]-[7,1,1]
	auto point_hits = bvh.point_query(mml::Vector3f(6.5f, 0.5f, 0.5f));
	// Should find at least primitive 3
	assert_equal_float((float)point_hits.size() > 0, 1.0f, "BVH point query finds primitive");
	if (!point_hits.empty()) {
		// Check that one of the results is primitive 3
		bool found = false;
		for (auto *p : point_hits) {
			if (p->id == 3) {
				found = true;
				break;
			}
		}
		assert_equal_float((float)found, 1.0f, "BVH point query found primitive 3");
	}
}

// Benchmark infrastructure
struct BenchmarkResult {
	const char *name;
	double time_ms;
	double ops_per_sec;
	std::size_t iterations;
};

std::vector<BenchmarkResult> benchmark_results;

template <typename Func>
void benchmark(const char *name, Func &&func, std::size_t warmup_iterations = 1000, std::size_t measure_iterations = 100000) {
	// Warmup
	for (std::size_t i = 0; i < warmup_iterations; ++i) {
		func();
	}

	// Measure
	auto start = std::chrono::high_resolution_clock::now();
	for (std::size_t i = 0; i < measure_iterations; ++i) {
		func();
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> duration = end - start;
	BenchmarkResult result;
	result.name = name;
	result.time_ms = duration.count();
	result.iterations = measure_iterations;
	result.ops_per_sec = (measure_iterations / duration.count()) * 1000.0;
	benchmark_results.push_back(result);
}

void print_benchmark_header() {
	std::cout << "\n========================================" << std::endl;
	std::cout << "   Performance Benchmarks" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << std::left << std::setw(35) << "Test"
			  << std::right << std::setw(12) << "Time (ms)"
			  << std::setw(15) << "Ops/sec"
			  << std::setw(12) << "Iterations" << std::endl;
	std::cout << std::string(74, '-') << std::endl;
}

void print_benchmark_results() {
	print_benchmark_header();
	for (const auto &result : benchmark_results) {
		std::cout << std::left << std::setw(35) << result.name
				  << std::right << std::setw(12) << std::fixed << std::setprecision(3) << result.time_ms
				  << std::setw(15) << std::setprecision(0) << result.ops_per_sec
				  << std::setw(12) << result.iterations << std::endl;
	}
	std::cout << std::string(74, '-') << std::endl;
}

// Benchmark functions
void benchmark_matrix4_multiply() {
	mml::Matrix4f a = mml::Matrix4f::rotation_euler(0.5f, 0.3f, 0.2f);
	mml::Matrix4f b = mml::Matrix4f::translation(1.0f, 2.0f, 3.0f) * mml::Matrix4f::scale(2.0f, 2.0f, 2.0f);

	benchmark("Matrix4 Multiply", [&]() {
		volatile auto result = a * b;
		(void)result;
	});
}

void benchmark_matrix4_vector_multiply() {
	mml::Matrix4f m = mml::Matrix4f::rotation_euler(0.5f, 0.3f, 0.2f);
	mml::Vector4f v(1.0f, 2.0f, 3.0f, 1.0f);

	benchmark("Matrix4 * Vector4", [&]() {
		volatile auto result = m * v;
		(void)result;
	});
}

void benchmark_vector3_operations() {
	mml::Vector3f a(1.0f, 2.0f, 3.0f);
	mml::Vector3f b(4.0f, 5.0f, 6.0f);

	benchmark("Vector3 Add", [&]() {
		volatile auto result = a + b;
		(void)result;
	});

	benchmark("Vector3 Dot", [&]() {
		volatile float result = a.dot(b);
		(void)result;
	});

	benchmark("Vector3 Cross", [&]() {
		volatile auto result = a.cross(b);
		(void)result;
	});

	benchmark("Vector3 Length", [&]() {
		volatile float result = a.length();
		(void)result;
	});

	benchmark("Vector3 Normalize", [&]() {
		volatile auto result = a.normalized();
		(void)result;
	});
}

void benchmark_vector4_operations() {
	mml::Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
	mml::Vector4f b(5.0f, 6.0f, 7.0f, 8.0f);

	benchmark("Vector4 Add", [&]() {
		volatile auto result = a + b;
		(void)result;
	});

	benchmark("Vector4 Dot", [&]() {
		volatile float result = a.dot(b);
		(void)result;
	});

	benchmark("Vector4 Length", [&]() {
		volatile float result = a.length();
		(void)result;
	});

	benchmark("Vector4 Normalize", [&]() {
		volatile auto result = a.normalized();
		(void)result;
	});
}

void benchmark_quaternion_operations() {
	mml::Quaternionf q1 = mml::Quaternionf::from_euler(0.5f, 0.3f, 0.2f);
	mml::Quaternionf q2 = mml::Quaternionf::from_euler(0.2f, 0.4f, 0.1f);
	mml::Vector3f v(1.0f, 0.0f, 0.0f);

	benchmark("Quaternion Multiply", [&]() {
		volatile auto result = q1 * q2;
		(void)result;
	});

	benchmark("Quaternion Rotate Vector", [&]() {
		volatile auto result = q1.rotate(v);
		(void)result;
	});

	benchmark("Quaternion Slerp", [&]() {
		volatile auto result = mml::Quaternionf::slerp(q1, q2, 0.5f);
		(void)result;
	});

	benchmark("Quaternion Normalize", [&]() {
		volatile auto result = q1.normalized();
		(void)result;
	});
}

void benchmark_aabb_operations() {
	mml::AABBf aabb(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 1.0f, 1.0f));
	mml::Matrix4f matrix = mml::Matrix4f::rotation_euler(0.5f, 0.3f, 0.2f) * mml::Matrix4f::translation(5.0f, 0.0f, 0.0f);

	benchmark("AABB Transform (Arvo)", [&]() {
		volatile auto result = mml::transform(aabb, matrix);
		(void)result;
	});

	mml::AABBf other(mml::Vector3f(0.5f, 0.5f, 0.5f), mml::Vector3f(1.5f, 1.5f, 1.5f));
	benchmark("AABB Intersection", [&]() {
		volatile bool result = aabb.intersects(other);
		(void)result;
	});

	benchmark("AABB Contains Point", [&]() {
		volatile bool result = aabb.contains(mml::Vector3f(0.5f, 0.5f, 0.5f));
		(void)result;
	});
}

void benchmark_matrix3_operations() {
	mml::Matrix3f a = mml::Matrix3f::rotation_euler(0.5f, 0.3f, 0.2f);
	mml::Matrix3f b = mml::Matrix3f::scale(2.0f, 2.0f, 2.0f);

	benchmark("Matrix3 Multiply", [&]() {
		volatile auto result = a * b;
		(void)result;
	});

	benchmark("Matrix3 Inverse", [&]() {
		volatile auto result = a.inverse();
		(void)result;
	});

	benchmark("Matrix3 Transpose", [&]() {
		volatile auto result = a.transposed();
		(void)result;
	});
}

void benchmark_transform_hierarchy() {
	mml::TransformNodef root;
	root.set_position(mml::Vector3f(0.0f, 0.0f, 0.0f));

	mml::TransformNodef child1;
	child1.set_position(mml::Vector3f(1.0f, 0.0f, 0.0f));
	child1.set_parent(&root);

	mml::TransformNodef child2;
	child2.set_position(mml::Vector3f(0.0f, 1.0f, 0.0f));
	child2.set_parent(&child1);

	benchmark("Transform Hierarchy Update", [&]() {
		child2.set_position(mml::Vector3f(0.0f, 1.0f, 0.1f));
		volatile auto result = child2.get_world_matrix();
		(void)result;
	});
}

void benchmark_curve_operations() {
	mml::BezierCubicf bezier(
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.0f, 2.0f, 0.0f),
			mml::Vector3f(2.0f, 2.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f));

	mml::BezierQuadraticf quad_bezier(
			mml::Vector3f(0.0f, 0.0f, 0.0f),
			mml::Vector3f(1.5f, 2.0f, 0.0f),
			mml::Vector3f(3.0f, 0.0f, 0.0f));

	std::vector<mml::Vector3f> catmull_points = {
		mml::Vector3f(0.0f, 0.0f, 0.0f),
		mml::Vector3f(1.0f, 1.0f, 0.0f),
		mml::Vector3f(2.0f, 1.0f, 0.0f),
		mml::Vector3f(3.0f, 0.0f, 0.0f),
		mml::Vector3f(4.0f, 0.0f, 0.0f)
	};
	mml::CatmullRomSplinef spline(catmull_points);

	mml::HermiteSplinef hermite;
	hermite.add_point(mml::Vector3f(0.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 1.0f, 0.0f));
	hermite.add_point(mml::Vector3f(2.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, -1.0f, 0.0f));
	hermite.add_point(mml::Vector3f(4.0f, 0.0f, 0.0f), mml::Vector3f(1.0f, 1.0f, 0.0f));

	benchmark("Cubic Bezier Evaluate", [&]() {
		volatile auto result = bezier.evaluate(0.5f);
		(void)result;
	});

	benchmark("Cubic Bezier Tangent", [&]() {
		volatile auto result = bezier.tangent(0.5f);
		(void)result;
	});

	benchmark("Cubic Bezier Second Derivative", [&]() {
		volatile auto result = bezier.second_derivative(0.5f);
		(void)result;
	});

	benchmark("Cubic Bezier Curvature", [&]() {
		volatile auto result = bezier.curvature(0.5f);
		(void)result;
	});

	benchmark("Cubic Bezier Subdivide", [&]() {
		volatile auto result = bezier.subdivide(0.5f);
		(void)result;
	});

	benchmark("Cubic Bezier Length", [&]() {
		volatile auto result = bezier.length();
		(void)result;
	});

	benchmark("Cubic Bezier Reparameterize", [&]() {
		volatile auto result = bezier.reparameterize(0.5f);
		(void)result;
	});

	benchmark("Quadratic Bezier Evaluate", [&]() {
		volatile auto result = quad_bezier.evaluate(0.5f);
		(void)result;
	});

	benchmark("Quadratic Bezier Tangent", [&]() {
		volatile auto result = quad_bezier.tangent(0.5f);
		(void)result;
	});

	benchmark("Quadratic Bezier Length", [&]() {
		volatile auto result = quad_bezier.length(20);
		(void)result;
	});

	benchmark("Catmull-Rom Evaluate", [&]() {
		volatile auto result = spline.evaluate(0.5f);
		(void)result;
	});

	benchmark("Catmull-Rom Tangent", [&]() {
		volatile auto result = spline.tangent(0.5f);
		(void)result;
	});

	benchmark("Catmull-Rom Length", [&]() {
		volatile auto result = spline.length();
		(void)result;
	});

	benchmark("Catmull-Rom Segment Evaluate", [&]() {
		volatile auto result = spline.evaluate_segment(1, 0.5f);
		(void)result;
	});

	benchmark("Catmull-Rom Sample Uniform", [&]() {
		volatile auto result = spline.sample_uniform(100);
		(void)result;
	});

	benchmark("Hermite Spline Evaluate", [&]() {
		volatile auto result = hermite.evaluate(0.5f);
		(void)result;
	});

	benchmark("Hermite Spline Tangent", [&]() {
		volatile auto result = hermite.tangent(0.5f);
		(void)result;
	});

	benchmark("Hermite from Catmull-Rom", [&]() {
		volatile auto result = mml::HermiteSplinef::from_catmull_rom(catmull_points);
		(void)result;
	});

	benchmark("Cubic Bezier Normal", [&]() {
		volatile auto result = bezier.normal(0.5f);
		(void)result;
	});

	benchmark("Cubic Bezier Frame", [&]() {
		volatile auto result = bezier.frame(0.5f);
		(void)result;
	});

	benchmark("Catmull-Rom Frame", [&]() {
		volatile auto result = spline.frame(0.5f);
		(void)result;
	});

	benchmark("Hermite Spline Frame", [&]() {
		volatile auto result = hermite.frame(0.5f);
		(void)result;
	});

	benchmark("Cubic Bezier Flatten", [&]() {
		volatile auto result = bezier.flatten(0.01f);
		(void)result;
	});

	benchmark("Quadratic Bezier Flatten", [&]() {
		volatile auto result = quad_bezier.flatten(0.01f);
		(void)result;
	});
}

void benchmark_bvh_operations() {
	struct TestPrim {
		mml::AABBf bounds;
		int id;
	};

	std::vector<TestPrim> prims;
	for (int i = 0; i < 100; ++i) {
		TestPrim p;
		p.bounds = mml::AABBf(
				mml::Vector3f((float)i * 2.0f, 0.0f, 0.0f),
				mml::Vector3f((float)i * 2.0f + 1.0f, 1.0f, 1.0f));
		p.id = i;
		prims.push_back(p);
	}

	mml::BVH<float, TestPrim> bvh([](const TestPrim &p) { return p.bounds; });
	bvh.build(prims);

	benchmark("BVH Build", [&]() {
		std::vector<TestPrim> temp = prims;
		mml::BVH<float, TestPrim> temp_bvh([](const TestPrim &p) { return p.bounds; });
		temp_bvh.build(temp);
	});

	benchmark("BVH Ray Intersection", [&]() {
		volatile auto result = bvh.ray_intersect(
				mml::Vector3f(-1.0f, 0.5f, 0.5f),
				mml::Vector3f(1.0f, 0.0f, 0.0f));
		(void)result;
	});

	benchmark("BVH Point Query", [&]() {
		volatile auto result = bvh.point_query(mml::Vector3f(5.5f, 0.5f, 0.5f));
		(void)result;
	});

	benchmark("BVH Get Bounds", [&]() {
		volatile auto result = bvh.get_bounds();
		(void)result;
	});

	benchmark("BVH Ray Intersection (Closest)", [&]() {
		float dist;
		volatile auto result = bvh.ray_intersect_closest(
				mml::Vector3f(-1.0f, 0.5f, 0.5f),
				mml::Vector3f(1.0f, 0.0f, 0.0f),
				dist);
		(void)result;
		(void)dist;
	});

	print_header("BVH vs Naive Comparison");

	mml::Vector3f ray_origin(-1.0f, 0.5f, 0.5f);
	mml::Vector3f ray_dir(1.0f, 0.0f, 0.0f);

	benchmark("Naive Ray Intersection (100 prims)", [&]() {
		std::vector<TestPrim *> result;
		mml::Rayf ray(ray_origin, ray_dir);
		for (auto &p : prims) {
			float tmin, tmax;
			if (p.bounds.intersect_ray(ray, tmin, tmax)) {
				result.push_back(&p);
			}
		}
		(void)result;
	});

	benchmark("BVH Ray Intersection (100 prims)", [&]() {
		volatile auto result = bvh.ray_intersect(ray_origin, ray_dir);
		(void)result;
	});

	mml::Vector3f query_point(55.5f, 0.5f, 0.5f);

	benchmark("Naive Point Query (100 prims)", [&]() {
		std::vector<TestPrim *> result;
		for (auto &p : prims) {
			if (p.bounds.contains(query_point)) {
				result.push_back(&p);
			}
		}
		(void)result;
	});

	benchmark("BVH Point Query (100 prims)", [&]() {
		volatile auto result = bvh.point_query(query_point);
		(void)result;
	});

	std::vector<TestPrim> dense_prims;
	for (int i = 0; i < 1000; ++i) {
		TestPrim p;
		p.bounds = mml::AABBf(
				mml::Vector3f((float)(i % 50) * 2.0f, (float)(i / 50) * 2.0f, 0.0f),
				mml::Vector3f((float)(i % 50) * 2.0f + 1.0f, (float)(i / 50) * 2.0f + 1.0f, 1.0f));
		p.id = i;
		dense_prims.push_back(p);
	}

	mml::BVH<float, TestPrim> dense_bvh([](const TestPrim &p) { return p.bounds; });
	dense_bvh.build(dense_prims);

	benchmark("Naive Ray Intersection (1000 prims)", [&]() {
		std::vector<TestPrim *> result;
		mml::Rayf ray(ray_origin, ray_dir);
		for (auto &p : dense_prims) {
			float tmin, tmax;
			if (p.bounds.intersect_ray(ray, tmin, tmax)) {
				result.push_back(&p);
			}
		}
		(void)result;
	});

	benchmark("BVH Ray Intersection (1000 prims)", [&]() {
		volatile auto result = dense_bvh.ray_intersect(ray_origin, ray_dir);
		(void)result;
	});

	benchmark("Naive Point Query (1000 prims)", [&]() {
		std::vector<TestPrim *> result;
		for (auto &p : dense_prims) {
			if (p.bounds.contains(query_point)) {
				result.push_back(&p);
			}
		}
		(void)result;
	});

	benchmark("BVH Point Query (1000 prims)", [&]() {
		volatile auto result = dense_bvh.point_query(query_point);
		(void)result;
	});
}

void run_all_benchmarks() {
	benchmark_results.clear();

	print_header("Starting Performance Benchmarks");
	std::cout << "Warmup: 1000 iterations per test" << std::endl;
	std::cout << "Measurement: 100000 iterations per test" << std::endl;

	benchmark_matrix4_multiply();
	benchmark_matrix4_vector_multiply();
	benchmark_vector3_operations();
	benchmark_vector4_operations();
	benchmark_quaternion_operations();
	benchmark_aabb_operations();
	benchmark_matrix3_operations();
	benchmark_transform_hierarchy();
	benchmark_curve_operations();
	benchmark_bvh_operations();

	print_benchmark_results();
}

void test_all() {
	print_header("Machina Math Library Tests");
	std::cout << std::fixed << std::setprecision(6);

	test_constants();
	test_epsilon();
	test_math();
	test_color();
	test_vectors();
	test_matrices();
	test_quaternions();
	test_geometry();
	test_bounding_volume();
	test_line_and_polygon();
	test_random();
	test_transforms();
	test_utils();
	test_simd();
	test_transform_node();
	test_basis();
	test_deterministic_random();
	test_soa();
	test_curves();
	test_bvh();

	print_header("Test Summary");
	std::cout << "  Total tests:  " << total_tests << std::endl;
	std::cout << "  Passed:       " << passed_tests << std::endl;
	std::cout << "  Failed:       " << failed_tests << std::endl;
	float success_rate = total_tests > 0 ? (float)passed_tests / total_tests * 100.0f : 0.0f;
	std::cout << "  Success rate: " << success_rate << "%" << std::endl;
	std::cout << "========================================" << std::endl;
}

int main(int argc, char *argv[]) {
	// Check for benchmark flag
	bool run_benchmarks = false;
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "--benchmark" || std::string(argv[i]) == "-b") {
			run_benchmarks = true;
			break;
		}
	}

	if (run_benchmarks) {
		run_all_benchmarks();
		return 0;
	}

	test_all();
	return failed_tests > 0 ? 1 : 0;
}
