#include "geomcore/geomcore.hpp"
#include "geomcore/topology.hpp"

#include <algorithm>
#include <map>
#include <utility>

namespace geomcore {

const char* version() noexcept { return "0.1.0"; }

VertexId Polyhedron::add_vertex(const Vec3& p) {
    vertices_.push_back(p);
    return static_cast<VertexId>(vertices_.size() - 1);
}

FaceId Polyhedron::add_face(const Polygon& poly) {
    faces_.push_back(poly);
    return static_cast<FaceId>(faces_.size() - 1);
}

void Polyhedron::build_adjacency() {
    edges_.clear();

    // Map an undirected vertex pair to an index in edges_.
    std::map<std::pair<VertexId, VertexId>, std::size_t> lookup;

    for (FaceId f = 0; f < faces_.size(); ++f) {
        const Polygon& poly = faces_[f];
        const std::size_t n = poly.loop.size();
        for (std::size_t i = 0; i < n; ++i) {
            const VertexId a = poly.loop[i];
            const VertexId b = poly.loop[(i + 1) % n];
            const bool forward = a < b;
            const std::pair<VertexId, VertexId> key =
                forward ? std::make_pair(a, b) : std::make_pair(b, a);

            auto it = lookup.find(key);
            if (it == lookup.end()) {
                EdgeAdjacency e;
                e.v0 = key.first;
                e.v1 = key.second;
                // Directed half-edge a->b: orientation picks left vs right.
                if (forward) e.left_face = f; else e.right_face = f;
                lookup.emplace(key, edges_.size());
                edges_.push_back(e);
            } else {
                EdgeAdjacency& e = edges_[it->second];
                if (forward) e.left_face = f; else e.right_face = f;
            }
        }
    }
}

bool Polyhedron::is_closed_manifold() const noexcept {
    if (edges_.empty()) return false;
    for (const EdgeAdjacency& e : edges_) {
        if (!e.is_manifold()) return false;
    }
    return true;
}

} // namespace geomcore
