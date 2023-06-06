#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

class Treap {
private:
    static std::mt19937 random_;
    class Node {
    public:
        const int value_;
        const int priority_;
        Node* left_;
        Node* right_;

        explicit Node(int);
        ~Node();
    };
    std::size_t size_;
    Node* root_;

    [[nodiscard]] bool contains(const Node* node, int value) const noexcept;
    void values(const Node* node, std::vector<int>& result) const noexcept;

    [[nodiscard]] Node* remove(Node* node, int value);

    [[nodiscard]] std::pair<Node*, Node*> split(Node* node, int value);
    [[nodiscard]] Node* merge(Node* left, Node* right);

public:
    [[nodiscard]] bool contains(int value) const noexcept;
    bool insert(int value);
    bool remove(int value);

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] std::vector<int> values() const noexcept;

    Treap();
    ~Treap();
};

#endif
