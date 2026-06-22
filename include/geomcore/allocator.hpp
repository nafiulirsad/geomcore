#pragma once
//
// allocator.hpp
// Memory management hook point for a custom allocator. (Requirement 4)
//
// All long-lived geometry/topology containers route their allocations through
// MemoryResource so a host application can swap in pooled/arena strategies
// without touching the geometry algorithms.
//
#include <cstddef>

namespace geomcore {

// Minimal allocation interface (intentionally not std::pmr to keep the public
// boundary dependency-light). A host supplies a concrete implementation.
class MemoryResource {
public:
    virtual ~MemoryResource() = default;
    virtual void* allocate(std::size_t bytes, std::size_t alignment) = 0;
    virtual void deallocate(void* ptr, std::size_t bytes, std::size_t alignment) = 0;
};

// Returns the default (malloc-backed) resource.
MemoryResource* default_resource() noexcept;

// Install a custom resource; returns the previously installed one.
// This is the single hook point for plugging in a custom allocator.
MemoryResource* set_default_resource(MemoryResource* resource) noexcept;

} // namespace geomcore
