#include "geomcore/mat.hpp"

namespace geomcore {

Mat4 Mat4::translation(const Vec3& t) noexcept {
    Mat4 r;  // identity
    r.m[3][0] = t.x;
    r.m[3][1] = t.y;
    r.m[3][2] = t.z;
    return r;
}

Mat4 Mat4::scale(const Vec3& s) noexcept {
    Mat4 r;  // identity
    r.m[0][0] = s.x;
    r.m[1][1] = s.y;
    r.m[2][2] = s.z;
    return r;
}

Mat4 Mat4::operator*(const Mat4& rhs) const noexcept {
    Mat4 out;
    for (int c = 0; c < 4; ++c) {
        for (int rrow = 0; rrow < 4; ++rrow) {
            Real sum = 0.0;
            for (int k = 0; k < 4; ++k) {
                sum += m[k][rrow] * rhs.m[c][k];
            }
            out.m[c][rrow] = sum;
        }
    }
    return out;
}

Vec3 Mat4::transform_point(const Vec3& p) const noexcept {
    return {
        m[0][0] * p.x + m[1][0] * p.y + m[2][0] * p.z + m[3][0],
        m[0][1] * p.x + m[1][1] * p.y + m[2][1] * p.z + m[3][1],
        m[0][2] * p.x + m[1][2] * p.y + m[2][2] * p.z + m[3][2],
    };
}

Vec3 Mat4::transform_dir(const Vec3& d) const noexcept {
    return {
        m[0][0] * d.x + m[1][0] * d.y + m[2][0] * d.z,
        m[0][1] * d.x + m[1][1] * d.y + m[2][1] * d.z,
        m[0][2] * d.x + m[1][2] * d.y + m[2][2] * d.z,
    };
}

bool Mat4::is_finite() const noexcept {
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            if (!geomcore::is_finite(m[c][r])) return false;
    return true;
}

bool Mat4::nearly_equal(const Mat4& o, const Tolerance& t) const noexcept {
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            if (!geomcore::nearly_equal(m[c][r], o.m[c][r], t)) return false;
    return true;
}

} // namespace geomcore
