#ifndef PUZZLE_SOLVER_HPP
#define PUZZLE_SOLVER_HPP

#include <map>
#include <set>

#include "puzzle/Board.hpp"

class Solver {
    class Solution {
    public:
        std::size_t moves() const { return m_moves.empty() ? 0 : m_moves.size() - 1; }

        using const_iterator = std::vector<Board>::const_iterator;

        const_iterator begin() const { return m_moves.begin(); }

        const_iterator end() const { return m_moves.end(); }

    private:
        friend class Solver;

        std::vector<Board> m_moves;
    };

    class Node {
    public:
        Node(std::vector<std::vector<unsigned>>&& data, unsigned depth = 0)
            : _board(std::forward<std::vector<std::vector<unsigned>>>(data)), _depth(depth) {}
        explicit Node(const Board& board) : _board(board), _depth() {}

        const Board _board;
        const unsigned _depth;
    };

    static std::vector<Node> next_pos(const Node& node);

public:
    static Solution solve(const Board& initial);
};

#endif  // PUZZLE_SOLVER_HPP
