#include "geomcore/allocator.hpp"

#include <cstdlib>
#include <new>

namespace geomcore {
namespace {

// Default malloc-backed resource honoring alignment.
class MallocResource final : public MemoryResource {
public:
    void* allocate(std::size_t bytes, std::size_t alignment) override {
        if (alignment < alignof(void*)) alignment = alignof(void*);
        void* p = nullptr;
#if defined(_MSC_VER)
        p = _aligned_malloc(bytes, alignment);
#else
        // size must be a multiple of alignment for std::aligned_alloc.
        const std::size_t rounded = (bytes + alignment - 1) / alignment * alignment;
        p = std::aligned_alloc(alignment, rounded);
#endif
        if (!p) throw std::bad_alloc();
        return p;
    }

    void deallocate(void* ptr, std::size_t, std::size_t) override {
#if defined(_MSC_VER)
        _aligned_free(ptr);
#else
        std::free(ptr);
#endif
    }
};

MallocResource g_malloc_resource;
MemoryResource* g_current = &g_malloc_resource;

} // namespace

MemoryResource* default_resource() noexcept { return g_current; }

MemoryResource* set_default_resource(MemoryResource* resource) noexcept {
    MemoryResource* prev = g_current;
    g_current = resource ? resource : &g_malloc_resource;
    return prev;
}

} // namespace geomcore
