#include "geomcore/tolerance.hpp"

#include <algorithm>

namespace geomcore {

const Tolerance& default_tolerance() noexcept {
    static const Tolerance kDefault{};
    return kDefault;
}

bool nearly_equal(Real a, Real b, const Tolerance& tol) noexcept {
    const Real diff = std::fabs(a - b);
    if (diff <= tol.abs_eps) return true;
    const Real scale = std::max(std::fabs(a), std::fabs(b));
    return diff <= tol.rel_eps * scale;
}

bool nearly_zero(Real v, const Tolerance& tol) noexcept {
    return std::fabs(v) <= tol.abs_eps;
}

int compare(Real a, Real b, const Tolerance& tol) noexcept {
    if (nearly_equal(a, b, tol)) return 0;
    return (a < b) ? -1 : 1;
}

} // namespace geomcore
