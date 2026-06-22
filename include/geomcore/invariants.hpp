#pragma once
//
// invariants.hpp
// Invariant enforcement + failure-mode detection. (Requirement 5)
//
#include "geomcore/topology.hpp"
#include "geomcore/vec.hpp"

namespace geomcore {

// Classified failure modes for geometric/topological validation.
enum class FailureMode {
    None,
    Degenerate,    // zero-area face, collinear loop, < 3 vertices, etc.
    NonFinite,     // NaN or +/-inf coordinate
    NonManifold,   // edge shared by != 2 faces (when closed manifold expected)
    Empty          // no geometry to validate
};

// Lightweight result type carried across the validation boundary.
struct ValidationResult {
    bool        ok   = true;
    FailureMode mode = FailureMode::None;
    const char* detail = "";

    explicit operator bool() const noexcept { return ok; }

    static ValidationResult success() noexcept { return {true, FailureMode::None, ""}; }
    static ValidationResult failure(FailureMode m, const char* d) noexcept {
        return {false, m, d};
    }
};

// Failure-mode detectors.
bool has_non_finite(const Vec3& v) noexcept;
bool is_degenerate(const Polygon& poly, const Polyhedron& mesh,
                   const Tolerance& tol = default_tolerance()) noexcept;

// Full invariant check over a polyhedron: finiteness, degeneracy, manifoldness.
ValidationResult validate(const Polyhedron& mesh,
                          const Tolerance& tol = default_tolerance());

} // namespace geomcore
