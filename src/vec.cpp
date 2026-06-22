#include "geomcore/vec.hpp"

namespace geomcore {

bool Vec2::nearly_equal(const Vec2& o, const Tolerance& t) const noexcept {
    return geomcore::nearly_equal(x, o.x, t) && geomcore::nearly_equal(y, o.y, t);
}

bool Vec3::nearly_equal(const Vec3& o, const Tolerance& t) const noexcept {
    return geomcore::nearly_equal(x, o.x, t) &&
           geomcore::nearly_equal(y, o.y, t) &&
           geomcore::nearly_equal(z, o.z, t);
}

} // namespace geomcore
