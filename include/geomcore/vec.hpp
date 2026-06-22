#pragma once
//
// vec.hpp
// 2D / 3D coordinate types. (Requirement 1)
//
#include "geomcore/tolerance.hpp"

namespace geomcore {

struct Vec2 {
    Real x = 0.0;
    Real y = 0.0;

    Vec2() = default;
    Vec2(Real x_, Real y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& o) const noexcept { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const noexcept { return {x - o.x, y - o.y}; }
    Vec2 operator*(Real s) const noexcept { return {x * s, y * s}; }

    Real dot(const Vec2& o) const noexcept { return x * o.x + y * o.y; }
    Real length_sq() const noexcept { return dot(*this); }
    Real length() const noexcept { return std::sqrt(length_sq()); }

    // Tolerance-aware equality (no raw ==).
    bool nearly_equal(const Vec2& o, const Tolerance& t = default_tolerance()) const noexcept;
    bool is_finite() const noexcept { return geomcore::is_finite(x) && geomcore::is_finite(y); }
};

struct Vec3 {
    Real x = 0.0;
    Real y = 0.0;
    Real z = 0.0;

    Vec3() = default;
    Vec3(Real x_, Real y_, Real z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& o) const noexcept { return {x + o.x, y + o.y, z + o.z}; }
    Vec3 operator-(const Vec3& o) const noexcept { return {x - o.x, y - o.y, z - o.z}; }
    Vec3 operator*(Real s) const noexcept { return {x * s, y * s, z * s}; }

    Real dot(const Vec3& o) const noexcept { return x * o.x + y * o.y + z * o.z; }
    Vec3 cross(const Vec3& o) const noexcept {
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
    }
    Real length_sq() const noexcept { return dot(*this); }
    Real length() const noexcept { return std::sqrt(length_sq()); }

    // Tolerance-aware equality (no raw ==).
    bool nearly_equal(const Vec3& o, const Tolerance& t = default_tolerance()) const noexcept;
    bool is_finite() const noexcept {
        return geomcore::is_finite(x) && geomcore::is_finite(y) && geomcore::is_finite(z);
    }
};

} // namespace geomcore
