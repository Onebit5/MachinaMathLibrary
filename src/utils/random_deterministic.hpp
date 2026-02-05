/**************************************************************************/
/*  random_deterministic.hpp                                              */
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

#include "../core/constants.hpp"
#include "../core/traits.hpp"
#include <cstdint>
#include <limits>
#include <type_traits>

namespace mml {

namespace random {

// ============================================================================
// Linear Congruential Generator (LCG)
// Simple, fast, decent quality for non-cryptographic use
// ============================================================================

class LCG32 {
public:
	using result_type = uint32_t;

	static constexpr uint32_t default_seed = 123456789u;

	// LCG parameters (Numerical Recipes)
	static constexpr uint64_t multiplier = 1664525ull;
	static constexpr uint64_t increment = 1013904223ull;
	static constexpr uint64_t modulus = 4294967296ull; // 2^32

	explicit LCG32(uint32_t seed = default_seed) :
			state_(seed), initial_seed_(seed) {}

	void seed(uint32_t s) {
		state_ = s;
		initial_seed_ = s;
	}

	uint32_t operator()() {
		state_ = static_cast<uint32_t>((multiplier * state_ + increment) % modulus);
		return state_;
	}

	uint32_t next_uint32() { return (*this)(); }

	uint64_t next_uint64() {
		uint64_t high = (*this)();
		uint64_t low = (*this)();
		return (high << 32) | low;
	}

	// Generate float in [0, 1)
	float next_float() {
		return static_cast<float>((*this)()) * (1.0f / 4294967296.0f);
	}

	// Generate double in [0, 1)
	double next_double() {
		return static_cast<double>((*this)()) * (1.0 / 4294967296.0);
	}

	// Generate value in range [min, max]
	uint32_t range_uint32(uint32_t min_val, uint32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = max_val - min_val + 1;
		return min_val + ((*this)() % range);
	}

	int32_t range_int32(int32_t min_val, int32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = static_cast<uint32_t>(max_val - min_val + 1);
		return min_val + static_cast<int32_t>((*this)() % range);
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	// Boolean with probability p
	bool chance(float probability) {
		return next_float() < probability;
	}

	// Reset to seed
	void reset() { state_ = initial_seed_; }

	static constexpr uint32_t min() { return 0; }
	static constexpr uint32_t max() { return std::numeric_limits<uint32_t>::max(); }

private:
	uint32_t state_;
	uint32_t initial_seed_;
};

// ============================================================================
// PCG32 - Permuted Congruential Generator
// Higher quality than LCG, still fast, excellent statistical properties
// ============================================================================

class PCG32 {
public:
	using result_type = uint32_t;

	static constexpr uint64_t default_seed = 0x853c49e6748fea9bull;
	static constexpr uint64_t default_stream = 0xda3e39cb94b95bdbull;

	explicit PCG32(uint64_t seed = default_seed, uint64_t stream = default_stream) :
			initial_seed_(seed), initial_stream_(stream) {
		seed_(seed, stream);
	}

	void seed(uint64_t s, uint64_t stream = default_stream) {
		initial_seed_ = s;
		initial_stream_ = stream;
		seed_(s, stream);
	}

	uint32_t operator()() {
		uint64_t oldstate = state_;
		state_ = oldstate * 6364136223846793005ull + (inc_ | 1);
		uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
		uint32_t rot = static_cast<uint32_t>(oldstate >> 59u);
		return (xorshifted >> rot) | (xorshifted << ((-static_cast<int32_t>(rot)) & 31));
	}

	uint32_t next_uint32() { return (*this)(); }

	uint64_t next_uint64() {
		uint64_t high = (*this)();
		uint64_t low = (*this)();
		return (high << 32) | low;
	}

	float next_float() {
		return static_cast<float>((*this)()) * (1.0f / 4294967296.0f);
	}

	double next_double() {
		// Use 53 bits of precision for doubles
		uint32_t high = (*this)();
		uint32_t low = (*this)();
		uint64_t val = (static_cast<uint64_t>(high) << 32) | low;
		return static_cast<double>(val >> 11) * (1.0 / 9007199254740992.0);
	}

	uint32_t range_uint32(uint32_t min_val, uint32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = max_val - min_val + 1;

		// Rejection sampling for uniform distribution
		uint32_t threshold = (std::numeric_limits<uint32_t>::max() / range) * range;
		uint32_t result;
		do {
			result = (*this)();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	int32_t range_int32(int32_t min_val, int32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		return static_cast<int32_t>(range_uint32(0, static_cast<uint32_t>(max_val - min_val))) + min_val;
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	bool chance(float probability) {
		return next_float() < probability;
	}

	void reset() { seed_(initial_seed_, initial_stream_); }

	static constexpr uint32_t min() { return 0; }
	static constexpr uint32_t max() { return std::numeric_limits<uint32_t>::max(); }

private:
	uint64_t state_;
	uint64_t inc_;
	uint64_t initial_seed_;
	uint64_t initial_stream_;

	void seed_(uint64_t seed, uint64_t stream) {
		state_ = 0u;
		inc_ = (stream << 1u) | 1u;
		(*this)();
		state_ += seed;
		(*this)();
	}
};

// ============================================================================
// Xoroshiro128+ - Fast, good statistical properties
// Part of the xoshiro/xoroshiro family by Sebastiano Vigna
// ============================================================================

class Xoroshiro128Plus {
public:
	using result_type = uint64_t;

	static constexpr uint64_t default_seed = 0x9e3779b97f4a7c15ull;

	explicit Xoroshiro128Plus(uint64_t seed = default_seed) :
			initial_seed_(seed) {
		seed_(seed);
	}

	void seed(uint64_t s) {
		initial_seed_ = s;
		seed_(s);
	}

	uint64_t operator()() {
		const uint64_t s0 = s_[0];
		uint64_t s1 = s_[1];
		const uint64_t result = s0 + s1;

		s1 ^= s0;
		s_[0] = rotl_(s0, 55) ^ s1 ^ (s1 << 14);
		s_[1] = rotl_(s1, 36);

		return result;
	}

	uint32_t next_uint32() {
		return static_cast<uint32_t>((*this)());
	}

	uint64_t next_uint64() { return (*this)(); }

	float next_float() {
		// Use upper 24 bits for float
		return static_cast<float>((*this)() >> 40) * (1.0f / 16777216.0f);
	}

	double next_double() {
		// Use upper 53 bits for double
		return static_cast<double>((*this)() >> 11) * (1.0 / 9007199254740992.0);
	}

	uint32_t range_uint32(uint32_t min_val, uint32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint32_t range = max_val - min_val + 1;

		uint32_t threshold = (std::numeric_limits<uint32_t>::max() / range) * range;
		uint32_t result;
		do {
			result = next_uint32();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	int32_t range_int32(int32_t min_val, int32_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		return static_cast<int32_t>(range_uint32(0, static_cast<uint32_t>(max_val - min_val))) + min_val;
	}

	uint64_t range_uint64(uint64_t min_val, uint64_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint64_t range = max_val - min_val + 1;

		uint64_t threshold = (std::numeric_limits<uint64_t>::max() / range) * range;
		uint64_t result;
		do {
			result = (*this)();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	int64_t range_int64(int64_t min_val, int64_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		return static_cast<int64_t>(range_uint64(0, static_cast<uint64_t>(max_val - min_val))) + min_val;
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	bool chance(float probability) {
		return next_float() < probability;
	}

	// Jump ahead by 2^64 calls - useful for parallel streams
	void jump() {
		static const uint64_t JUMP[] = { 0xbeac0467eba5facb, 0xd86b048b86aa9922 };

		uint64_t s0 = 0;
		uint64_t s1 = 0;

		for (int i = 0; i < 2; ++i) {
			for (int b = 0; b < 64; ++b) {
				if ((JUMP[i] >> b) & 1) {
					s0 ^= s_[0];
					s1 ^= s_[1];
				}
				(*this)();
			}
		}

		s_[0] = s0;
		s_[1] = s1;
	}

	void reset() { seed_(initial_seed_); }

	static constexpr uint64_t min() { return 0; }
	static constexpr uint64_t max() { return std::numeric_limits<uint64_t>::max(); }

private:
	uint64_t s_[2];
	uint64_t initial_seed_;

	static uint64_t rotl_(uint64_t x, int k) {
		return (x << k) | (x >> (64 - k));
	}

	void seed_(uint64_t seed) {
		// SplitMix64 to initialize state
		uint64_t z = seed + 0x9e3779b97f4a7c15ull;
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		s_[0] = z ^ (z >> 31);

		z = s_[0] + 0x9e3779b97f4a7c15ull;
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		s_[1] = z ^ (z >> 31);
	}
};

// ============================================================================
// SplitMix64 - Used for seeding other generators
// Also a decent generator on its own
// ============================================================================

class SplitMix64 {
public:
	using result_type = uint64_t;

	static constexpr uint64_t default_seed = 0x9e3779b97f4a7c15ull;

	explicit SplitMix64(uint64_t seed = default_seed) :
			state_(seed), initial_seed_(seed) {}

	void seed(uint64_t s) {
		state_ = s;
		initial_seed_ = s;
	}

	uint64_t operator()() {
		uint64_t z = (state_ += 0x9e3779b97f4a7c15ull);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		return z ^ (z >> 31);
	}

	uint32_t next_uint32() {
		return static_cast<uint32_t>((*this)());
	}

	uint64_t next_uint64() { return (*this)(); }

	float next_float() {
		return static_cast<float>((*this)() >> 40) * (1.0f / 16777216.0f);
	}

	double next_double() {
		return static_cast<double>((*this)() >> 11) * (1.0 / 9007199254740992.0);
	}

	uint64_t range_uint64(uint64_t min_val, uint64_t max_val) {
		if (min_val >= max_val) {
			return min_val;
		}
		uint64_t range = max_val - min_val + 1;

		uint64_t threshold = (std::numeric_limits<uint64_t>::max() / range) * range;
		uint64_t result;
		do {
			result = (*this)();
		} while (result >= threshold);

		return min_val + (result % range);
	}

	float range_float(float min_val, float max_val) {
		return min_val + next_float() * (max_val - min_val);
	}

	double range_double(double min_val, double max_val) {
		return min_val + next_double() * (max_val - min_val);
	}

	bool chance(float probability) {
		return next_float() < probability;
	}

	void reset() { state_ = initial_seed_; }

	static constexpr uint64_t min() { return 0; }
	static constexpr uint64_t max() { return std::numeric_limits<uint64_t>::max(); }

private:
	uint64_t state_;
	uint64_t initial_seed_;
};

// ============================================================================
// Type Aliases
// ============================================================================

using RandomLCG = LCG32;
using RandomPCG = PCG32;
using RandomXoroshiro = Xoroshiro128Plus;
using RandomSplitMix = SplitMix64;

} // namespace random

} // namespace mml