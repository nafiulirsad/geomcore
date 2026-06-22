# GeomCore — C++17 3D Geometry & Topology Engine

High-performance C++17 library for BIM geometric operations: 2D/3D coordinate
systems, transformation matrices, polygon/polyhedron topology, and rigorous
numerical tolerance handling. Designed with clean public interfaces for
invariant enforcement and failure-mode detection, intended for consumption by
downstream BIM visualization and validation systems.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
# or run the smoke test directly:
./build/geomcore_tests
```

Requires CMake >= 3.16 and a C++17 compiler. `CMAKE_CXX_STANDARD` is pinned to
17 with extensions off.

## Requirement → code map

| # | Requirement | Where |
|---|-------------|-------|
| 1 | 2D/3D coordinate types + transform matrix | `include/geomcore/vec.hpp` (`Vec2`, `Vec3`), `include/geomcore/mat.hpp` (`Mat4`) |
| 2 | Polygon/polyhedron topology w/ adjacency & connectivity | `include/geomcore/topology.hpp` (`Polygon`, `Polyhedron`, `EdgeAdjacency`, `build_adjacency`, `is_closed_manifold`) |
| 3 | Epsilon-aware compare (no raw `==` on floats) | `include/geomcore/tolerance.hpp` (`nearly_equal`, `nearly_zero`, `compare`) |
| 4 | Custom allocator hook point | `include/geomcore/allocator.hpp` (`MemoryResource`, `set_default_resource`) |
| 5 | Invariant enforcement + failure-mode detection | `include/geomcore/invariants.hpp` (`FailureMode`, `ValidationResult`, `validate`) — detects degenerate, NaN/inf, non-manifold |
| 6 | Clean public interface for downstream viz/validation | `include/geomcore/geomcore.hpp` (umbrella header) |

## Project hard rules (GeomCore)

- **Tolerance compare, never raw `==` on floats.** All floating-point equality
  and ordering routes through `geomcore::nearly_equal` / `compare` / `nearly_zero`.
  Vector and matrix equality use these internally.
- **Failure modes are classified, not asserted away.** `validate()` returns a
  `ValidationResult` carrying a `FailureMode` (`Degenerate`, `NonFinite`,
  `NonManifold`, `Empty`).
- **Single memory hook point** so a host can install a pooled/arena allocator
  via `set_default_resource` without touching geometry algorithms.

## Layout

```
CMakeLists.txt           # lib + test exe, C++17
include/geomcore/*.hpp   # public domain interfaces (#pragma once)
src/*.cpp                # implementations
tests/test_main.cpp      # assert-based smoke test, no external framework
```
