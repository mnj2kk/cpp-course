#include "puzzle/Solver.hpp"

std::vector<Solver::Node> Solver::next_pos(const Solver::Node& node) {
    const auto [i, j] = node._board._empty;
    std::vector<Solver::Node> result;
    if (i != 0) {
        auto data = node._board._data;
        std::swap(data[i][j], data[i - 1][j]);
        result.emplace_back(std::move(data), node._depth + 1);
    }
    if (i + 1 < node._board._size) {
        auto data = node._board._data;
        std::swap(data[i][j], data[i + 1][j]);
        result.emplace_back(std::move(data), node._depth + 1);
    }
    if (j != 0) {
        auto data = node._board._data;
        std::swap(data[i][j], data[i][j - 1]);
        result.emplace_back(std::move(data), node._depth + 1);
    }
    if (j + 1 < node._board._size) {
        auto data = node._board._data;
        std::swap(data[i][j], data[i][j + 1]);
        result.emplace_back(std::move(data), node._depth + 1);
    }
    return result;
}

Solver::Solution Solver::solve(const Board& board) {
    Solution solution;
    if (!board.is_solvable()) {
        return solution;
    }
    const auto absolute_compare = [](const Board& lhs, const Board& rhs) {
        if (lhs._size != rhs._size) {
            return lhs._size < rhs._size;
        }
        if (lhs._hamming != rhs._hamming) {
            return lhs._hamming < rhs._hamming;
        }
        if (lhs._manhattan != rhs._manhattan) {
            return lhs._manhattan < rhs._manhattan;
        }
        if (lhs._empty != rhs._empty) {
            return lhs._empty < rhs._empty;
        }
        if (lhs._inversion != rhs._inversion) {
            return lhs._inversion < rhs._inversion;
        }
        return lhs._data < rhs._data;
    };
    const auto heuristic = [](const Node& lhs, const Node& rhs) {
        if (lhs._board._manhattan + lhs._depth / 2.71828 != rhs._board._manhattan + rhs._depth / 2.71828) {
            return lhs._board._manhattan + lhs._depth / 2.71828 < rhs._board._manhattan + rhs._depth / 2.71828;
        }
        if (lhs._board._inversion + lhs._board._hamming != rhs._board._inversion + rhs._board._hamming) {
            return lhs._board._inversion + lhs._board._hamming < rhs._board._inversion + rhs._board._hamming;
        }
        if (lhs._board._size != rhs._board._size) {
            return lhs._board._size < rhs._board._size;
        }
        return lhs._board._empty < rhs._board._empty;
    };
    std::map<Board, Board, decltype(absolute_compare)> parent;
    std::map<Board, unsigned, decltype(absolute_compare)> used;
    std::set<Node, decltype(heuristic)> queue;
    used.insert({board, 0});
    queue.insert(Node(board));
    Board end;
    while (!queue.empty()) {
        const auto cur = *queue.begin();
        if (queue.begin()->_board.is_goal()) {
            end = queue.begin()->_board;
            break;
        }
        queue.erase(queue.begin());
        const auto next = next_pos(cur);
        for (const auto& pos : next) {
            if (!used.contains(pos._board) || used[pos._board] > pos._depth) {
                used[pos._board]   = pos._depth;
                parent[pos._board] = cur._board;
                queue.insert(pos);
            }
        }
    }
    solution.m_moves.reserve(used[end] + 1);
    solution.m_moves.emplace_back(end);
    while (end != board) {
        end = parent[end];
        solution.m_moves.emplace_back(end);
    }
    std::reverse(solution.m_moves.begin(), solution.m_moves.end());
    return solution;
}
