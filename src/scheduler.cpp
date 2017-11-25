#include "internal.h"

namespace asyncxx {
namespace detail {

void* aligned_alloc(std::size_t size, std::size_t align) {
#ifdef _WIN32
	void* ptr = _aligned_malloc(size, align);
	if (!ptr)
		LIBASYNC_THROW(std::bad_alloc());
	return ptr;
#else
	void* result;
	if (posix_memalign(&result, align, size))
		LIBASYNC_THROW(std::bad_alloc());
	else
		return result;
#endif
}

void aligned_free(void* addr) LIBASYNC_NOEXCEPT {
#ifdef _WIN32
	_aligned_free(addr);
#else
	free(addr);
#endif
}

}
};