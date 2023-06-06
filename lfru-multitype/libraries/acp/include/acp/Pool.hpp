#ifndef ACP_POOL_HPP
#define ACP_POOL_HPP

#include <cstddef>
#include <initializer_list>
#include <new>
#include <unordered_map>
#include <vector>

class PoolAllocator {
private:
    using pos_type = std::pair<std::size_t, std::size_t>;
    inline static constexpr pos_type invalid_pos{static_cast<std::size_t>(-1), static_cast<std::size_t>(-1)};
    std::vector<std::vector<std::byte>> _data;
    std::vector<std::vector<bool>> _used;
    std::unordered_map<std::size_t, std::size_t> _block_pos;

    pos_type find_free(std::size_t const size);

public:
    PoolAllocator(std::size_t const block_size, std::initializer_list<std::size_t> list);

    void* allocate(std::size_t const size);

    void deallocate(void const* ptr);
};

#endif  // ACP_POOL_HPP
