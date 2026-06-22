#pragma once
//
// mat.hpp
// 4x4 transformation matrix (column-major). (Requirement 1)
//
#include "geomcore/vec.hpp"

namespace geomcore {

struct Mat4 {
    // Column-major storage: m[col][row].
    Real m[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

    static Mat4 identity() noexcept { return Mat4{}; }
    static Mat4 translation(const Vec3& t) noexcept;
    static Mat4 scale(const Vec3& s) noexcept;

    // Matrix product (this * rhs).
    Mat4 operator*(const Mat4& rhs) const noexcept;

    // Transform a point (assumes w = 1, applies translation).
    Vec3 transform_point(const Vec3& p) const noexcept;
    // Transform a direction (w = 0, ignores translation).
    Vec3 transform_dir(const Vec3& d) const noexcept;

    bool is_finite() const noexcept;
    bool nearly_equal(const Mat4& o, const Tolerance& t = default_tolerance()) const noexcept;
};

} // namespace geomcore
