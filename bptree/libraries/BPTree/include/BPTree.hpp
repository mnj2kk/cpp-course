#ifndef BPTREE_HPP
#define BPTREE_HPP

#include <functional>
#include <stdexcept>
#include <vector>

template <class Key, class Value, std::size_t BlockSize = 4096, class Less = std::less<Key>>
class BPTree {
public:
    using key_type        = Key;
    using mapped_type     = Value;
    using value_type      = std::pair<Key, Value>;  // NB: a digression from std::map
    using reference       = value_type &;
    using const_reference = const value_type &;
    using pointer         = value_type *;
    using const_pointer   = const value_type *;
    using size_type       = std::size_t;

private:
    struct Parent {
        Parent *parent;

        Parent(Parent *parent = nullptr) : parent(parent) {}
    };
    struct Data: Parent {
        pointer content;

        Data(pointer content = nullptr, Parent *parent = nullptr) : Parent(parent), content(content) {}

        ~Data() { delete content; }
    };
    struct Node: Parent {
        bool leaf;
        pointer maximum_key;
        std::vector<Parent *> child;

        Node(bool leaf = false) : Parent(), leaf(leaf), maximum_key(), child() {}

        ~Node() {
            for (Parent *next : child) {
                if (leaf) {
                    delete static_cast<Data *>(next);
                } else {
                    delete static_cast<Node *>(next);
                }
            }
        }
    };
    struct Root: Node {
        Data *end;

        Root() : Node(true), end(new Data(nullptr, this)) {}
        ~Root() { delete end; }
    };

    template <bool CONST>
    struct Iterator {
    private:
        using T = value_type;
        friend class BPTree;
        Data *_data;
        Iterator(Data *data) : _data(data) {}

        inline static std::function<std::pair<Parent *, bool>(std::size_t, std::vector<Parent *>)> increment_next =
            [](const std::size_t pos, const std::vector<Parent *> &child) {
                if (pos + 1 < child.size()) {
                    return std::make_pair(child[pos + 1], true);
                }
                return std::make_pair(static_cast<Parent *>(nullptr), false);
            };
        inline static std::function<std::pair<Parent *, bool>(std::size_t, std::vector<Parent *>)> decrement_next =
            [](const std::size_t pos, const std::vector<Parent *> &child) {
                if (pos != 0) {
                    return std::make_pair(child[pos - 1], true);
                }
                return std::make_pair(static_cast<Parent *>(nullptr), false);
            };
        inline static std::function<Parent *(std::vector<Parent *>)> increment_downgrade =
            [](const std::vector<Parent *> &child) { return child.front(); };
        inline static std::function<Parent *(std::vector<Parent *>)> decrement_downgrade =
            [](const std::vector<Parent *> &child) { return child.back(); };

        void abstract_operator(const std::function<std::pair<Parent *, bool>(std::size_t, std::vector<Parent *>)> &next,
                               const std::function<Parent *(std::vector<Parent *>)> &downgrade) {
            Node *node    = static_cast<Node *>(_data->parent);
            size_type pos = 0;
            for (; pos < node->child.size(); ++pos) {
                if (node->child[pos] == _data) {
                    break;
                }
            }
            if (pos == node->child.size()) {
                while (!node->leaf) {
                    node = static_cast<Node *>(downgrade(node->child));
                }
                _data = static_cast<Data *>(downgrade(node->child));
                return;
            }
            std::pair<Parent *, bool> result = next(pos, node->child);
            if (result.second) {
                _data = static_cast<Data *>(result.first);
                return;
            }
            Node *parent = node;
            while ((parent = static_cast<Node *>(parent->parent)) != nullptr) {
                pos = 0;
                for (; pos < parent->child.size(); ++pos) {
                    if (parent->child[pos] == node) {
                        break;
                    }
                }
                result = next(pos, parent->child);
                if (result.second) {
                    break;
                }
                node = parent;
            }
            if (parent == nullptr) {
                _data = static_cast<Root *>(node)->end;
                return;
            }
            node = static_cast<Node *>(result.first);
            while (!node->leaf) {
                node = static_cast<Node *>(downgrade(node->child));
            }
            _data = static_cast<Data *>(downgrade(node->child));
        }

    public:
        using value_type        = T;
        using reference         = typename std::conditional<CONST, T const &, T &>::type;
        using pointer           = typename std::conditional<CONST, T const *, T *>::type;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        Iterator() : _data() {}
        Iterator(const Iterator &other) = default;
        template <bool _CONST = CONST, class = std::enable_if_t<_CONST>>
        Iterator(const Iterator<false> &other) : _data(other._data) {}

        Iterator &operator=(const Iterator &other) = default;

        pointer operator->() const { return _data->content; }
        reference operator*() const { return *this->operator->(); }

        template <bool _CONST>
        bool operator==(const Iterator<_CONST> &other) const {
            return _data == other._data;
        }

        Iterator &operator++() {
            abstract_operator(increment_next, increment_downgrade);
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++*this;
            return temp;
        }

        Iterator &operator--() {
            abstract_operator(decrement_next, decrement_downgrade);
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --*this;
            return temp;
        }
    };

public:
    using iterator       = Iterator<false>;
    using const_iterator = Iterator<true>;

private:
    using cmp                     = std::function<bool(Key, Key)>;
    inline static Less _less      = Less{};
    inline static cmp less        = [](const Key &a, const Key &b) { return _less(a, b); };
    inline static cmp equal       = [](const Key &a, const Key &b) { return !(less(a, b) || less(b, a)); };
    inline static cmp less_equal  = [](const Key &a, const Key &b) { return less(a, b) || equal(a, b); };
    inline static cmp great       = [](const Key &a, const Key &b) { return !less_equal(a, b); };
    inline static cmp great_equal = [](const Key &a, const Key &b) { return !less(a, b); };

    Node *find_leaf(const Key &key, const cmp &depth_compare) const {
        Node *node = _root;
        while (!node->leaf) {
            size_type pos = 0;
            while (pos < node->child.size() &&
                   depth_compare(static_cast<Node *>(node->child[pos])->maximum_key->first, key)) {
                ++pos;
            }
            if (pos == node->child.size()) {
                --pos;
            }
            node = static_cast<Node *>(node->child[pos]);
        }
        return node;
    }
    std::pair<iterator, bool> abstract_find(const Key &key, const cmp &depth_compare, const cmp &find_compare) const {
        Node *leaf = find_leaf(key, depth_compare);
        for (Parent *data : leaf->child) {
            if (find_compare(key, static_cast<Data *>(data)->content->first)) {
                return {iterator(static_cast<Data *>(data)), true};
            }
        }
        return {iterator(_end), false};
    }
    std::pair<iterator, bool> abstract_find(const Key &key) const { return abstract_find(key, less, equal); }
    iterator abstract_lower(const Key &key) const { return abstract_find(key, less, less_equal).first; }
    iterator abstract_upper(const Key &key) const { return abstract_find(key, less_equal, less).first; }
    std::pair<iterator, iterator> abstract_range(const Key &key) const {
        return {abstract_lower(key), abstract_upper(key)};
    }
    Value &abstract_at(const Key &key) const {
        std::pair<iterator, bool> result = abstract_find(key);
        if (!result.second) {
            throw std::out_of_range("No such key in BPTree.");
        }
        return result.first->second;
    }
    void update(Node *node) {
        while ((node = static_cast<Node *>(node->parent)) != nullptr) {
            node->maximum_key = static_cast<Node *>(node->child.back())->maximum_key;
        }
    }
    Node *update(Node *left, Node *right) {
        if (left->parent == nullptr) {
            Node *node        = new Node(left->leaf);
            node->parent      = _root;
            node->maximum_key = left->maximum_key;
            node->child       = std::move(left->child);
            left              = node;
            for (Parent *parent : left->child) {
                parent->parent = left;
            }
            _root->leaf        = false;
            _root->maximum_key = left->maximum_key;
            _root->child       = {};
            _root->child.push_back(left);
        }
        Node *parent  = static_cast<Node *>(left->parent);
        right->parent = parent;
        size_type pos = find_child(parent, left) + 1;
        parent->child.insert(parent->child.begin() + pos, right);
        parent->maximum_key = static_cast<Node *>(parent->child.back())->maximum_key;
        return parent;
    }
    void update_maximum(Node *node) {
        if (node->child.empty()) {
            node->maximum_key = nullptr;
            return;
        }
        if (node->leaf) {
            node->maximum_key = static_cast<Data *>(node->child.back())->content;
        } else {
            node->maximum_key = static_cast<Node *>(node->child.back())->maximum_key;
        }
    }
    void split(Node *node) {
        Node *right = new Node(node->leaf);
        right->child.reserve(_children_cnt);
        for (size_type i = _children_cnt; i < (_children_cnt << 1); ++i) {
            right->child.push_back(node->child[i]);
            right->child.back()->parent = right;
        }
        node->child.resize(_children_cnt);
        update_maximum(node);
        update_maximum(right);
        Node *parent = update(node, right);
        if (parent->child.size() == (_children_cnt << 1)) {
            split(parent);
        }
    }
    std::pair<iterator, bool> abstract_insert(Data *data) {
        Node *leaf    = find_leaf(data->content->first, less);
        size_type pos = 0;
        while (pos < leaf->child.size() &&
               less(static_cast<Data *>(leaf->child[pos])->content->first, data->content->first)) {
            ++pos;
        }
        if (pos != leaf->child.size() &&
            equal(static_cast<Data *>(leaf->child[pos])->content->first, data->content->first)) {
            delete data;
            return {iterator(static_cast<Data *>(leaf->child[pos])), false};
        }
        if (_begin == _end || (pos != leaf->child.size() && _begin == leaf->child[pos])) {
            _begin = data;
        }
        ++_size;
        data->parent = leaf;
        leaf->child.insert(leaf->child.begin() + pos, data);
        leaf->maximum_key = static_cast<Data *>(leaf->child.back())->content;
        if (leaf->child.size() == (_children_cnt << 1)) {
            split(leaf);
        }
        update(leaf);
        return {iterator(data), true};
    }
    size_type find_child(Node *parent, Parent *child) const {
        size_type pos = 0;
        for (; pos < parent->child.size(); ++pos) {
            if (parent->child[pos] == child) {
                break;
            }
        }
        return pos;
    }
    void balance(Node *node) {
        Node *parent = static_cast<Node *>(node->parent);
        if (parent == nullptr) {
            return;
        }
        if (parent->parent == nullptr && parent->child.size() == 1) {
            _root->leaf        = node->leaf;
            _root->maximum_key = node->maximum_key;
            _root->child       = std::move(node->child);
            for (Parent *next : _root->child) {
                next->parent = _root;
            }
            delete node;
            return;
        }
        if (node->child.size() >= _children_cnt) {
            balance(parent);
            return;
        }
        size_type pos = find_child(parent, node);
        Node *left    = pos != 0 ? static_cast<Node *>(parent->child[pos - 1]) : nullptr;
        Node *right   = pos + 1 < parent->child.size() ? static_cast<Node *>(parent->child[pos + 1]) : nullptr;
        if (left != nullptr && left->child.size() > _children_cnt) {
            node->child.insert(node->child.begin(), left->child.back());
            left->child.back()->parent = node;
            left->child.pop_back();
            update_maximum(left);
            return;
        }
        if (right != nullptr && right->child.size() > _children_cnt) {
            node->child.push_back(right->child.front());
            right->child.front()->parent = node;
            right->child.erase(right->child.begin());
            update_maximum(node);
            return;
        }
        if (left != nullptr) {
            for (Parent *next : node->child) {
                left->child.push_back(next);
                next->parent = left;
            }
            node->child.clear();
            delete node;
            parent->child.erase(parent->child.begin() + pos);
            update_maximum(left);
            balance(parent);
            return;
        }
        if (right != nullptr) {
            for (Parent *next : right->child) {
                node->child.push_back(next);
                next->parent = node;
            }
            right->child.clear();
            delete right;
            parent->child.erase(parent->child.begin() + find_child(parent, right));
            update_maximum(node);
            balance(parent);
            return;
        }
    }
    void copy(Node *cur, Node *other) {
        cur->child.reserve(other->child.size());
        cur->leaf = other->leaf;
        for (Parent *next : other->child) {
            if (cur->leaf) {
                cur->child.push_back(new Data(new std::pair<Key, Value>(*static_cast<Data *>(next)->content)));
                if (_begin == _end) {
                    _begin = static_cast<Data *>(cur->child.back());
                }
            } else {
                cur->child.push_back(new Node());
                copy(static_cast<Node *>(cur->child.back()), static_cast<Node *>(next));
            }
            cur->child.back()->parent = cur;
        }
        update_maximum(cur);
    }
    void swap(BPTree &other) {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
        std::swap(_begin, other._begin);
        std::swap(_end, other._end);
    }

    Root *_root;
    size_type _size;
    Data *_begin;
    Data *_end;
    size_type _children_cnt;

public:
    BPTree()
        : _root(new Root())
        , _size()
        , _begin(_root->end)
        , _end(_root->end)
        , _children_cnt((BlockSize - sizeof(std::size_t) - sizeof(void *)) / (sizeof(Key) + sizeof(void *))) {}
    BPTree(std::initializer_list<std::pair<Key, Value>> list) : BPTree() { insert(list.begin(), list.end()); }
    BPTree(const BPTree &other)
        : _root(new Root())
        , _size(other._size)
        , _begin(_root->end)
        , _end(_root->end)
        , _children_cnt(other._children_cnt) {
        copy(_root, other._root);
    }
    BPTree(BPTree &&other)
        : _root(other._root)
        , _size(other._size)
        , _begin(other._begin)
        , _end(other._end)
        , _children_cnt(other._children_cnt) {
        other._root  = new Root();
        other._size  = 0;
        other._begin = other._root->end;
        other._end   = other._root->end;
    }

    BPTree &operator=(const BPTree &other) {
        BPTree temp = BPTree(other);
        swap(temp);
        return *this;
    }

    BPTree &operator=(BPTree &&other) {
        BPTree temp = BPTree(std::move(other));
        swap(temp);
        return *this;
    }

    ~BPTree() { delete _root; }

    iterator begin() { return _begin; }
    const_iterator cbegin() const { return begin(); }
    const_iterator begin() const { return _begin; }
    iterator end() { return _end; }
    const_iterator cend() const { return end(); }
    const_iterator end() const { return _end; }

    bool empty() const { return _size == 0; }
    size_type size() const { return _size; }
    void clear() { erase(_begin, _end); }

    size_type count(const Key &key) const { return contains(key) ? 1 : 0; }
    bool contains(const Key &key) const { return abstract_find(key).second; }
    std::pair<iterator, iterator> equal_range(const Key &key) { return abstract_range(key); }
    std::pair<const_iterator, const_iterator> equal_range(const Key &key) const { return abstract_range(key); }
    iterator lower_bound(const Key &key) { return abstract_lower(key); }
    const_iterator lower_bound(const Key &key) const { return abstract_lower(key); }
    iterator upper_bound(const Key &key) { return abstract_upper(key); }
    const_iterator upper_bound(const Key &key) const { return abstract_upper(key); }
    iterator find(const Key &key) { return abstract_find(key).first; }
    const_iterator find(const Key &key) const { return abstract_find(key).first; }

    // 'at' method throws std::out_of_range if there is no such key
    Value &at(const Key &key) { return abstract_at(key); }
    const Value &at(const Key &key) const { return abstract_at(key); }

    // '[]' operator inserts a new element if there is no such key
    Value &operator[](const Key &key) { return insert(key, Value{}).first->second; }

    std::pair<iterator, bool> insert(const Key &key, const Value &value) {
        Data *data = new Data(new std::pair<Key, Value>(key, value));
        return abstract_insert(data);
    }  // NB: a digression from std::map
    std::pair<iterator, bool> insert(const Key &key, Value &&value) {
        Data *data = new Data(new std::pair<Key, Value>(key, std::forward<Value>(value)));
        return abstract_insert(data);
    }  // NB: a digression from std::map
    template <class ForwardIt>
    void insert(ForwardIt begin, ForwardIt end) {
        for (ForwardIt it = begin; it != end; ++it) {
            insert(it->first, it->second);
        }
    }
    void insert(std::initializer_list<value_type> list) { return insert(list.begin(), list.end()); }
    iterator erase(const_iterator it) {
        const_iterator cur = it++;
        Data *data         = cur._data;
        if (cur == iterator(_begin)) {
            _begin = it._data;
        }
        Node *parent = static_cast<Node *>(data->parent);
        parent->child.erase(parent->child.begin() + find_child(parent, data));
        delete data;
        --_size;
        update_maximum(parent);
        balance(parent);
        return it._data;
    }
    iterator erase(const_iterator first, const_iterator last) {
        while (first != last) {
            erase(first++);
        }
        return last._data;
    }
    size_type erase(const Key &key) {
        std::pair<iterator, bool> result = abstract_find(key);
        if (!result.second) {
            return 0;
        }
        erase(result.first);
        return 1;
    }
};

#endif
