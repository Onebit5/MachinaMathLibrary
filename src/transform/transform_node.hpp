/**************************************************************************/
/*  transform_node.hpp                                                    */
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

#include "../algebra/matrix4.hpp"
#include "../algebra/quaternion.hpp"
#include "../algebra/vector3.hpp"
#include "../core/config.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace mml {

namespace transform_flags {
constexpr uint32_t POSITION_CHANGED = 1 << 0;
constexpr uint32_t ROTATION_CHANGED = 1 << 1;
constexpr uint32_t SCALE_CHANGED = 1 << 2;
constexpr uint32_t LOCAL_CHANGED = POSITION_CHANGED | ROTATION_CHANGED | SCALE_CHANGED;
constexpr uint32_t PARENT_CHANGED = 1 << 3;
constexpr uint32_t WORLD_CHANGED = LOCAL_CHANGED | PARENT_CHANGED;
} //namespace transform_flags

template <typename T>
struct TransformNode {
	Vector3<T> position;
	Quaternion<T> rotation;
	Vector3<T> scale;

private:
	mutable Matrix4<T> _local_matrix_cache;
	mutable Matrix4<T> _world_matrix_cache;
	mutable uint32_t _dirty_flags;
	TransformNode *_parent;
	std::vector<TransformNode *> _children;

public:
	TransformNode() :
			position(T(0), T(0), T(0)), rotation(Quaternion<T>::identity()), scale(T(1), T(1), T(1)), _local_matrix_cache(Matrix4<T>::identity()), _world_matrix_cache(Matrix4<T>::identity()), _dirty_flags(transform_flags::WORLD_CHANGED), _parent(nullptr) {}

	explicit TransformNode(const Vector3<T> &pos, const Quaternion<T> &rot, const Vector3<T> &scl) :
			position(pos), rotation(rot), scale(scl), _local_matrix_cache(Matrix4<T>::identity()), _world_matrix_cache(Matrix4<T>::identity()), _dirty_flags(transform_flags::WORLD_CHANGED), _parent(nullptr) {}

	TransformNode(const TransformNode &other) :
			position(other.position), rotation(other.rotation), scale(other.scale), _local_matrix_cache(other._local_matrix_cache), _world_matrix_cache(other._world_matrix_cache), _dirty_flags(other._dirty_flags), _parent(nullptr) {}

	~TransformNode() {
		set_parent(nullptr);
		for (TransformNode *child : _children) {
			if (child && child->_parent == this) {
				child->_parent = nullptr;
			}
		}
	}

	TransformNode &operator=(const TransformNode &other) {
		if (this != &other) {
			position = other.position;
			rotation = other.rotation;
			scale = other.scale;
			_local_matrix_cache = other._local_matrix_cache;
			_world_matrix_cache = other._world_matrix_cache;
			mark_dirty(transform_flags::LOCAL_CHANGED);
		}
		return *this;
	}

	static TransformNode identity() {
		return TransformNode();
	}

	void set_position(const Vector3<T> &pos) {
		if (pos != position) {
			position = pos;
			mark_dirty(transform_flags::POSITION_CHANGED);
		}
	}

	void set_position(T x, T y, T z) {
		set_position(Vector3<T>(x, y, z));
	}

	void set_rotation(const Quaternion<T> &rot) {
		if (rot != rotation) {
			rotation = rot;
			mark_dirty(transform_flags::ROTATION_CHANGED);
		}
	}

	void set_scale(const Vector3<T> &scl) {
		if (scl != scale) {
			scale = scl;
			mark_dirty(transform_flags::SCALE_CHANGED);
		}
	}

	void set_scale(T x, T y, T z) {
		set_scale(Vector3<T>(x, y, z));
	}

	void set_uniform_scale(T s) {
		set_scale(Vector3<T>(s, s, s));
	}

	void set_transform(const Vector3<T> &pos, const Quaternion<T> &rot, const Vector3<T> &scl) {
		position = pos;
		rotation = rot;
		scale = scl;
		mark_dirty(transform_flags::LOCAL_CHANGED);
	}

	MML_FORCE_INLINE const Vector3<T> &get_position() const { return position; }
	MML_FORCE_INLINE const Quaternion<T> &get_rotation() const { return rotation; }
	MML_FORCE_INLINE const Vector3<T> &get_scale() const { return scale; }

	MML_FORCE_INLINE Vector3<T> get_world_position() const {
		update_world_matrix();
		return Vector3<T>(_world_matrix_cache[3][0], _world_matrix_cache[3][1], _world_matrix_cache[3][2]);
	}

	MML_FORCE_INLINE Quaternion<T> get_world_rotation() const {
		update_world_matrix();
		Matrix4<T> parent_matrix = _parent ? _parent->get_world_matrix() : Matrix4<T>::identity();
		Matrix4<T> parent_inv = parent_matrix.inverse_affine();
		Matrix3<T> rot_matrix = (parent_inv * _world_matrix_cache).to_matrix3();
		return Quaternion<T>::from_matrix(rot_matrix);
	}

	MML_FORCE_INLINE Vector3<T> get_world_scale() const {
		update_world_matrix();
		Matrix4<T> parent_matrix = _parent ? _parent->get_world_matrix() : Matrix4<T>::identity();
		Matrix4<T> parent_inv = parent_matrix.inverse_affine();
		Matrix4<T> local_world = parent_inv * _world_matrix_cache;

		Vector3<T> scale_x(local_world[0][0], local_world[0][1], local_world[0][2]);
		Vector3<T> scale_y(local_world[1][0], local_world[1][1], local_world[1][2]);
		Vector3<T> scale_z(local_world[2][0], local_world[2][1], local_world[2][2]);

		return Vector3<T>(scale_x.length(), scale_y.length(), scale_z.length());
	}

	MML_FORCE_INLINE const Matrix4<T> &get_local_matrix() const {
		if (_dirty_flags & transform_flags::LOCAL_CHANGED) {
			_local_matrix_cache = compute_local_matrix();
			_dirty_flags &= ~transform_flags::LOCAL_CHANGED;
		}
		return _local_matrix_cache;
	}

	MML_FORCE_INLINE const Matrix4<T> &get_world_matrix() const {
		update_world_matrix();
		return _world_matrix_cache;
	}

	void update_world_matrix() const {
		if (!(_dirty_flags & transform_flags::WORLD_CHANGED)) {
			return;
		}

		if (_parent) {
			_parent->update_world_matrix();
			_world_matrix_cache = _parent->_world_matrix_cache * get_local_matrix();
		} else {
			_world_matrix_cache = get_local_matrix();
		}

		_dirty_flags &= ~transform_flags::WORLD_CHANGED;
	}

	void set_parent(TransformNode *parent) {
		if (_parent == parent) {
			return;
		}

		if (_parent) {
			auto it = std::find(_parent->_children.begin(), _parent->_children.end(), this);
			if (it != _parent->_children.end()) {
				_parent->_children.erase(it);
			}
		}

		_parent = parent;

		if (_parent) {
			_parent->_children.push_back(this);
		}

		mark_dirty(transform_flags::PARENT_CHANGED);
	}

	TransformNode *get_parent() const { return _parent; }
	const std::vector<TransformNode *> &get_children() const { return _children; }
	size_t get_child_count() const { return _children.size(); }

	MML_FORCE_INLINE Vector3<T> transform_point(const Vector3<T> &point) const {
		return get_world_matrix().transform_point(point);
	}

	MML_FORCE_INLINE Vector3<T> transform_direction(const Vector3<T> &direction) const {
		return get_world_matrix().transform_direction(direction);
	}

	MML_FORCE_INLINE Vector3<T> inverse_transform_point(const Vector3<T> &point) const {
		Matrix4<T> inv = get_world_matrix().inverse();
		return inv.transform_point(point);
	}

	MML_FORCE_INLINE Vector3<T> inverse_transform_direction(const Vector3<T> &direction) const {
		Matrix4<T> inv = get_world_matrix().inverse_affine();
		return inv.transform_direction(direction);
	}

	void mark_dirty(uint32_t flags) const {
		_dirty_flags |= flags;
		for (TransformNode *child : _children) {
			if (child) {
				child->mark_dirty(transform_flags::PARENT_CHANGED);
			}
		}
	}

	MML_FORCE_INLINE bool is_dirty() const { return _dirty_flags != 0; }
	MML_FORCE_INLINE uint32_t get_dirty_flags() const { return _dirty_flags; }
	MML_FORCE_INLINE void clear_dirty_flags() { _dirty_flags = 0; }

	static TransformNode look_at(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up) {
		Vector3<T> forward = (target - eye).normalized();
		Vector3<T> right = forward.cross(up).normalized();
		Vector3<T> new_up = right.cross(forward);

		Matrix3<T> rot_matrix(right, new_up, -forward);
		Quaternion<T> rot = Quaternion<T>::from_matrix(rot_matrix);

		TransformNode result;
		result.position = eye;
		result.rotation = rot;
		return result;
	}

	void translate(const Vector3<T> &delta) {
		set_position(position + delta);
	}

	void translate(T x, T y, T z) {
		translate(Vector3<T>(x, y, z));
	}

	void rotate(const Quaternion<T> &delta) {
		set_rotation(delta * rotation);
	}

	void rotate_axis(const Vector3<T> &axis, T angle) {
		Quaternion<T> delta = Quaternion<T>::from_axis_angle(axis, angle);
		rotate(delta);
	}

	void rotate_euler(T pitch, T yaw, T roll) {
		Quaternion<T> delta = Quaternion<T>::from_euler(pitch, yaw, roll);
		rotate(delta);
	}

	void rotate_around(const Vector3<T> &point, const Quaternion<T> &delta) {
		Vector3<T> offset = position - point;
		offset = delta.rotate(offset);
		position = point + offset;
		rotation = delta * rotation;
		mark_dirty(transform_flags::LOCAL_CHANGED);
	}

private:
	MML_FORCE_INLINE Matrix4<T> compute_local_matrix() const {
		Matrix4<T> s = Matrix4<T>::scale(scale);
		Matrix4<T> r = rotation.to_matrix4();
		Matrix4<T> t = Matrix4<T>::translation(position);
		return t * r * s;
	}
};

using TransformNodef = TransformNode<float>;
using TransformNoded = TransformNode<double>;

template <typename T>
struct TransformHierarchy {
	std::vector<std::unique_ptr<TransformNode<T>>> nodes;

	TransformNode<T> *create_node(const Vector3<T> &pos = Vector3<T>(),
			const Quaternion<T> &rot = Quaternion<T>::identity(),
			const Vector3<T> &scale = Vector3<T>(T(1), T(1), T(1))) {
		nodes.emplace_back(std::make_unique<TransformNode<T>>(pos, rot, scale));
		return nodes.back().get();
	}

	void clear() {
		nodes.clear();
	}

	void update_all() const {
		for (const auto &node : nodes) {
			if (node) {
				node->update_world_matrix();
			}
		}
	}

	size_t size() const { return nodes.size(); }

	TransformNode<T> *operator[](size_t index) { return nodes[index].get(); }
	const TransformNode<T> *operator[](size_t index) const { return nodes[index].get(); }
};

using TransformHierarchyf = TransformHierarchy<float>;
using TransformHierarchyd = TransformHierarchy<double>;

} //namespace mml