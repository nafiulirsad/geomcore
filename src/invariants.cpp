#include "geomcore/invariants.hpp"

namespace geomcore {

bool has_non_finite(const Vec3& v) noexcept {
    return !v.is_finite();
}

bool is_degenerate(const Polygon& poly, const Polyhedron& mesh,
                   const Tolerance& tol) noexcept {
    if (poly.is_degenerate()) return true;

    // Newell's method for signed area; near-zero area => degenerate.
    Vec3 normal{0, 0, 0};
    const std::size_t n = poly.loop.size();
    for (std::size_t i = 0; i < n; ++i) {
        const Vec3& a = mesh.vertex(poly.loop[i]);
        const Vec3& b = mesh.vertex(poly.loop[(i + 1) % n]);
        normal.x += (a.y - b.y) * (a.z + b.z);
        normal.y += (a.z - b.z) * (a.x + b.x);
        normal.z += (a.x - b.x) * (a.y + b.y);
    }
    return nearly_zero(normal.length(), tol);
}

ValidationResult validate(const Polyhedron& mesh, const Tolerance& tol) {
    if (mesh.vertex_count() == 0 || mesh.face_count() == 0) {
        return ValidationResult::failure(FailureMode::Empty, "no geometry");
    }

    // 1. Finiteness: reject NaN / inf coordinates.
    for (std::size_t i = 0; i < mesh.vertex_count(); ++i) {
        if (has_non_finite(mesh.vertex(static_cast<VertexId>(i)))) {
            return ValidationResult::failure(FailureMode::NonFinite,
                                             "non-finite vertex");
        }
    }

    // 2. Degeneracy: zero-area / collinear faces.
    for (std::size_t i = 0; i < mesh.face_count(); ++i) {
        if (is_degenerate(mesh.face(static_cast<FaceId>(i)), mesh, tol)) {
            return ValidationResult::failure(FailureMode::Degenerate,
                                             "degenerate face");
        }
    }

    // 3. Manifoldness: every edge shared by exactly two faces.
    if (!mesh.is_closed_manifold()) {
        return ValidationResult::failure(FailureMode::NonManifold,
                                         "non-manifold edge");
    }

    return ValidationResult::success();
}

} // namespace geomcore
