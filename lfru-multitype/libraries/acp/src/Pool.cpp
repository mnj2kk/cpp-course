#include "acp/Pool.hpp"

PoolAllocator::PoolAllocator(std::size_t const block_size, std::initializer_list<std::size_t> list)
    : _data(list.size(), std::vector<std::byte>(block_size)), _used(list.size()), _block_pos() {
    std::size_t pos = 0;
    for (std::size_t value_size : list) {
        _used[pos].resize(block_size / value_size);
        _block_pos[value_size] = pos++;
    }
}

void* PoolAllocator::allocate(std::size_t const size) {
    auto position = find_free(size);
    if (position == invalid_pos) {
        throw std::bad_alloc();
    }
    _used[position.first][position.second / size] = true;
    return &_data[position.first][position.second];
}

void PoolAllocator::deallocate(void const* ptr) {
    for (auto& [size, pos] : _block_pos) {
        for (std::size_t j = 0; j < _used[pos].size(); ++j) {
            if (_used[pos][j] && &_data[pos][j * size] == ptr) {
                _used[pos][j] = false;
                return;
            }
        }
    }
}

PoolAllocator::pos_type PoolAllocator::find_free(std::size_t const size) {
    if (!_block_pos.contains(size)) {
        return invalid_pos;
    }
    std::size_t i = _block_pos[size];
    for (std::size_t j = 0; j < _used[i].size(); ++j) {
        if (!_used[i][j]) {
            return std::make_pair(i, j * size);
        }
    }
    return invalid_pos;
}
