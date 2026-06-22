#pragma once
//
// tolerance.hpp
// Numerical tolerance utilities. HARD RULE: never compare floats with raw ==.
// All equality / ordering of floating point goes through these helpers.
//
#include <cmath>
#include <limits>

namespace geomcore {

using Real = double;

// Default absolute + relative epsilons tuned for BIM-scale geometry (mm..km).
struct Tolerance {
    Real abs_eps = 1e-9;
    Real rel_eps = 1e-9;
};

// Returns the process-wide default tolerance.
const Tolerance& default_tolerance() noexcept;

// True if a and b are equal within combined absolute/relative tolerance.
// This is THE replacement for `a == b` on floating point.
bool nearly_equal(Real a, Real b, const Tolerance& tol = default_tolerance()) noexcept;

// True if v is within tolerance of zero.
bool nearly_zero(Real v, const Tolerance& tol = default_tolerance()) noexcept;

// Tolerance-aware ordering: -1 if a < b, 0 if nearly_equal, +1 if a > b.
int compare(Real a, Real b, const Tolerance& tol = default_tolerance()) noexcept;

// Failure-mode detection primitive: rejects NaN and +/-inf.
inline bool is_finite(Real v) noexcept { return std::isfinite(v); }

} // namespace geomcore
