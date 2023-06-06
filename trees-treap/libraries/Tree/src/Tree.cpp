#include "tree/Tree.hpp"

Treap::Treap() : size_(0), root_(nullptr) {}

Treap::~Treap() {
    delete root_;
}

std::mt19937 Treap::random_ = std::mt19937(std::random_device{}());

Treap::Node::Node(int value) : value_(value), priority_(static_cast<int>(random_())), left_(nullptr), right_(nullptr) {}

Treap::Node::~Node() {
    delete left_;
    delete right_;
}

bool Treap::contains(const Treap::Node *const node, int value) const noexcept {
    if (node == nullptr) {
        return false;
    }
    if (value == node->value_) {
        return true;
    }
    if (value < node->value_) {
        return contains(node->left_, value);
    } else {
        return contains(node->right_, value);
    }
}

bool Treap::contains(int value) const noexcept {
    return contains(root_, value);
}

std::size_t Treap::size() const noexcept {
    return size_;
}

bool Treap::empty() const noexcept {
    return size_ == 0;
}

void Treap::values(const Treap::Node *const node, std::vector<int> &result) const noexcept {
    if (node == nullptr) {
        return;
    }
    values(node->left_, result);
    result.emplace_back(node->value_);
    values(node->right_, result);
}

std::vector<int> Treap::values() const noexcept {
    std::vector<int> result{};
    values(root_, result);
    return result;
}

bool Treap::insert(int value) {
    if (contains(value)) {
        return false;
    }
    const std::pair<Node *, Node *> split_treap = split(root_, value);
    ++size_;
    root_ = new Node(value);
    root_ = merge(split_treap.first, root_);
    root_ = merge(root_, split_treap.second);
    return true;
}

Treap::Node *Treap::remove(Treap::Node *const node, int value) {
    if (node->value_ == value) {
        Node *const result = merge(node->left_, node->right_);
        node->left_        = nullptr;
        node->right_       = nullptr;
        delete node;
        return result;
    }
    if (value < node->value_) {
        node->left_ = remove(node->left_, value);
    } else {
        node->right_ = remove(node->right_, value);
    }
    return node;
}

bool Treap::remove(int value) {
    if (root_ == nullptr || !contains(value)) {
        return false;
    }
    root_ = remove(root_, value);
    --size_;
    return true;
}

std::pair<Treap::Node *, Treap::Node *> Treap::split(Treap::Node *const node, int value) {
    if (node == nullptr) {
        return {nullptr, nullptr};
    }
    if (node->value_ < value) {
        std::pair<Treap::Node *, Treap::Node *> result = split(node->right_, value);
        node->right_                                   = result.first;
        return {node, result.second};
    } else {
        std::pair<Treap::Node *, Treap::Node *> result = split(node->left_, value);
        node->left_                                    = result.second;
        return {result.first, node};
    }
}

Treap::Node *Treap::merge(Treap::Node *const left, Treap::Node *const right) {
    if (right == nullptr) {
        return left;
    }
    if (left == nullptr) {
        return right;
    }
    if (right->priority_ < left->priority_) {
        left->right_ = merge(left->right_, right);
        return left;
    } else {
        right->left_ = merge(left, right->left_);
        return right;
    }
}
