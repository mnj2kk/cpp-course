#ifndef ACP_CACHE_HPP
#define ACP_CACHE_HPP

#include <cstddef>
#include <list>
#include <new>
#include <ostream>

template <class Key, class KeyProvider, class Allocator>
class Cache {
public:
    template <class... AllocArgs>
    Cache(const std::size_t cache_size, AllocArgs &&...alloc_args)
        : m_max_top_size(cache_size)
        , m_max_low_size(cache_size)
        , m_alloc(std::forward<AllocArgs>(alloc_args)...)
        , lfu()
        , queue() {}

    std::size_t size() const { return lfu.size() + queue.size(); }

    bool empty() const { return size() == 0; }

    template <class T>
    T &get(const Key &key);

    std::ostream &print(std::ostream &strm) const;

    friend std::ostream &operator<<(std::ostream &strm, const Cache &cache) { return cache.print(strm); }

private:
    const std::size_t m_max_top_size;
    const std::size_t m_max_low_size;
    Allocator m_alloc;
    std::list<KeyProvider *> lfu;
    std::list<KeyProvider *> queue;

    template <class T>
    void push_to_lfu(KeyProvider *key) {
        if (lfu.size() == m_max_top_size) {
            push_to_queue<T>(lfu.back());
            lfu.pop_back();
        }
        lfu.push_front(key);
    }

    template <class T>
    void push_to_queue(KeyProvider *key) {
        if (queue.size() == m_max_low_size) {
            m_alloc.template destroy<KeyProvider>(queue.back());
            queue.pop_back();
        }
        queue.push_front(key);
    }
};

template <class Key, class KeyProvider, class Allocator>
template <class T>
inline T &Cache<Key, KeyProvider, Allocator>::get(const Key &key) {
    const auto find_predicate = [key](KeyProvider* value) {
        return *value == key;
    };
    auto it = std::find_if(lfu.begin(), lfu.end(), find_predicate);
    if (it != lfu.end()) {
        lfu.splice(lfu.begin(), lfu, it);
        return *static_cast<T *>(lfu.front());
    }
    it = std::find_if(queue.begin(), queue.end(), find_predicate);
    if (it != queue.end()) {
        auto insert = *it;
        queue.erase(it);
        push_to_lfu<T>(insert);
        return *static_cast<T *>(lfu.front());
    }
    push_to_queue<T>(static_cast<T *>(m_alloc.template create<T>(T(key))));
    return *static_cast<T *>(queue.front());
}

template <class Key, class KeyProvider, class Allocator>
inline std::ostream &Cache<Key, KeyProvider, Allocator>::print(std::ostream &strm) const {
    return strm << "Priority: <empty>"
                << "\nRegular: <empty>"
                << "\n";
}

#endif  // ACP_CACHE_HPP
