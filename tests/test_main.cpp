//
// test_main.cpp
// Minimal smoke test (assert-based, no external framework).
//
#include "geomcore/geomcore.hpp"

#include <cassert>
#include <cstdio>
#include <limits>

using namespace geomcore;

namespace {

// Custom allocator to exercise the memory hook point (Requirement 4).
struct CountingResource final : MemoryResource {
    int allocs = 0;
    int frees  = 0;
    void* allocate(std::size_t bytes, std::size_t alignment) override {
        ++allocs;
        return default_resource()->allocate(bytes, alignment);
    }
    void deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        ++frees;
        default_resource()->deallocate(p, bytes, alignment);
    }
};

void test_tolerance() {
    // Requirement 3: epsilon-aware compare, never raw ==.
    assert(nearly_equal(0.1 + 0.2, 0.3));
    assert(!nearly_equal(1.0, 2.0));
    assert(nearly_zero(1e-12));
    assert(compare(1.0, 2.0) < 0);
    assert(compare(2.0, 1.0) > 0);
    assert(compare(1.0, 1.0) == 0);
    assert(!is_finite(std::numeric_limits<Real>::quiet_NaN()));
}

void test_vec_mat() {
    // Requirement 1: Vec2/Vec3 + Mat4.
    Vec2 a{1.0, 2.0};
    assert(a.nearly_equal(Vec2{1.0, 2.0}));

    Vec3 x{1, 0, 0};
    Vec3 y{0, 1, 0};
    assert(x.cross(y).nearly_equal(Vec3{0, 0, 1}));

    Mat4 t = Mat4::translation(Vec3{1, 2, 3});
    Vec3 p = t.transform_point(Vec3{0, 0, 0});
    assert(p.nearly_equal(Vec3{1, 2, 3}));

    // Direction ignores translation.
    Vec3 d = t.transform_dir(Vec3{1, 0, 0});
    assert(d.nearly_equal(Vec3{1, 0, 0}));

    Mat4 prod = Mat4::identity() * t;
    assert(prod.nearly_equal(t));
}

// Builds a unit tetrahedron (closed manifold).
Polyhedron make_tetra() {
    Polyhedron mesh;
    VertexId v0 = mesh.add_vertex({0, 0, 0});
    VertexId v1 = mesh.add_vertex({1, 0, 0});
    VertexId v2 = mesh.add_vertex({0, 1, 0});
    VertexId v3 = mesh.add_vertex({0, 0, 1});
    mesh.add_face(Polygon{{v0, v2, v1}});
    mesh.add_face(Polygon{{v0, v1, v3}});
    mesh.add_face(Polygon{{v1, v2, v3}});
    mesh.add_face(Polygon{{v2, v0, v3}});
    mesh.build_adjacency();
    return mesh;
}

void test_topology_and_invariants() {
    // Requirement 2 + 5.
    Polyhedron mesh = make_tetra();
    assert(mesh.vertex_count() == 4);
    assert(mesh.face_count() == 4);
    assert(mesh.edge_count() == 6);          // tetra: V - E + F = 2
    assert(mesh.is_closed_manifold());

    ValidationResult ok = validate(mesh);
    assert(ok);
    assert(ok.mode == FailureMode::None);

    // Degenerate face detection.
    Polyhedron deg;
    deg.add_vertex({0, 0, 0});
    deg.add_vertex({1, 0, 0});
    deg.add_vertex({2, 0, 0});               // collinear -> zero area
    deg.add_face(Polygon{{0, 1, 2}});
    deg.build_adjacency();
    ValidationResult dr = validate(deg);
    assert(!dr);
    assert(dr.mode == FailureMode::Degenerate);

    // Non-finite detection.
    Polyhedron bad = make_tetra();
    Polyhedron nan;
    nan.add_vertex({std::numeric_limits<Real>::infinity(), 0, 0});
    nan.add_vertex({1, 0, 0});
    nan.add_vertex({0, 1, 0});
    nan.add_face(Polygon{{0, 1, 2}});
    nan.build_adjacency();
    assert(validate(nan).mode == FailureMode::NonFinite);

    // Non-manifold detection: single open face.
    Polyhedron open;
    open.add_vertex({0, 0, 0});
    open.add_vertex({1, 0, 0});
    open.add_vertex({0, 1, 0});
    open.add_face(Polygon{{0, 1, 2}});
    open.build_adjacency();
    assert(validate(open).mode == FailureMode::NonManifold);
}

void test_allocator_hook() {
    // Requirement 4: swap the default resource.
    CountingResource counting;
    MemoryResource* prev = set_default_resource(default_resource());
    set_default_resource(prev);  // restore round-trip works

    void* p = counting.allocate(64, 16);
    assert(counting.allocs == 1);
    counting.deallocate(p, 64, 16);
    assert(counting.frees == 1);
}

} // namespace

int main() {
    test_tolerance();
    test_vec_mat();
    test_topology_and_invariants();
    test_allocator_hook();
    std::printf("GeomCore %s: all smoke tests passed\n", version());
    return 0;
}
