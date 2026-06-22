#pragma once
//
// geomcore.hpp
// Clean public umbrella interface consumed by downstream BIM visualization
// and validation systems. (Requirement 6)
//
// Downstream consumers include this single header.
//
#include "geomcore/tolerance.hpp"   // epsilon-aware compare (no raw == on floats)
#include "geomcore/vec.hpp"         // Vec2 / Vec3
#include "geomcore/mat.hpp"         // Mat4
#include "geomcore/allocator.hpp"   // custom allocator hook point
#include "geomcore/topology.hpp"    // Polygon / Polyhedron + adjacency
#include "geomcore/invariants.hpp"  // invariant enforcement + failure modes

namespace geomcore {

// Library version string.
const char* version() noexcept;

} // namespace geomcore
