#pragma once
//
// topology.hpp
// Polygon / polyhedron topology with adjacency & connectivity. (Requirement 2)
//
#include "geomcore/vec.hpp"
#include <cstdint>
#include <vector>

namespace geomcore {

using VertexId = std::uint32_t;
using EdgeId   = std::uint32_t;
using FaceId   = std::uint32_t;

inline constexpr std::uint32_t kInvalidId = 0xFFFFFFFFu;

// A planar polygon: an ordered loop of vertex indices into a coordinate buffer.
struct Polygon {
    std::vector<VertexId> loop;   // CCW ordering convention

    std::size_t vertex_count() const noexcept { return loop.size(); }
    bool is_degenerate() const noexcept { return loop.size() < 3; }
};

// Half-edge style adjacency record connecting faces across an edge.
struct EdgeAdjacency {
    VertexId v0 = kInvalidId;
    VertexId v1 = kInvalidId;
    FaceId   left_face  = kInvalidId;
    FaceId   right_face = kInvalidId;

    // An edge shared by exactly two faces is manifold.
    bool is_manifold() const noexcept {
        return left_face != kInvalidId && right_face != kInvalidId;
    }
    bool is_boundary() const noexcept {
        return (left_face == kInvalidId) != (right_face == kInvalidId);
    }
};

// A polyhedron: a shared vertex buffer plus faces and derived edge adjacency.
class Polyhedron {
public:
    VertexId add_vertex(const Vec3& p);
    FaceId   add_face(const Polygon& poly);

    const Vec3&    vertex(VertexId id) const { return vertices_[id]; }
    const Polygon& face(FaceId id) const { return faces_[id]; }

    std::size_t vertex_count() const noexcept { return vertices_.size(); }
    std::size_t face_count() const noexcept { return faces_.size(); }
    std::size_t edge_count() const noexcept { return edges_.size(); }

    // (Re)derive edge adjacency / connectivity from the current faces.
    void build_adjacency();
    const std::vector<EdgeAdjacency>& edges() const noexcept { return edges_; }

    // Connectivity query: every edge shared by exactly two faces.
    bool is_closed_manifold() const noexcept;

private:
    std::vector<Vec3>          vertices_;
    std::vector<Polygon>       faces_;
    std::vector<EdgeAdjacency> edges_;
};

} // namespace geomcore
