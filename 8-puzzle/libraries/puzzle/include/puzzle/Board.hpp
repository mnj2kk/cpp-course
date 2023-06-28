#ifndef PUZZLE_BOARD_HPP
#define PUZZLE_BOARD_HPP

#include <algorithm>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class Board {
private:
    std::vector<std::vector<unsigned>> _data;
    std::size_t _size;
    unsigned _hamming;
    unsigned _manhattan;
    unsigned _inversion;
    std::pair<std::size_t, std::size_t> _empty;

    thread_local static std::mt19937 _rnd;

    static std::pair<std::size_t, std::size_t> ceil_pos(unsigned ceil, std::size_t size);

    static std::size_t diff_abs(std::size_t a, std::size_t b);

    static unsigned hamming(const std::vector<std::vector<unsigned>>& board);
    static unsigned manhattan(const std::vector<std::vector<unsigned>>& board);
    static unsigned inversion(const std::vector<std::vector<unsigned>>& board);
    static std::pair<std::size_t, std::size_t> empty_pos(const std::vector<std::vector<unsigned>>& board);

    friend class Solver;

public:
    static Board create_goal(unsigned size);

    static Board create_random(unsigned size);

    Board();

    explicit Board(const std::vector<std::vector<unsigned>>& data);

    Board(std::vector<std::vector<unsigned>>&& data) noexcept;

    Board(const Board& board);

    Board(Board&& board) noexcept;

    std::size_t size() const;

    bool is_goal() const;

    unsigned hamming() const;

    unsigned manhattan() const;

    std::string to_string() const;

    bool is_solvable() const;

    const std::vector<unsigned>& operator[](const std::size_t index) const;

    Board& operator=(const Board& board);

    Board& operator=(Board&& board) noexcept;

    friend bool operator==(const Board& lhs, const Board& rhs) {
        if (lhs._size != rhs._size) {
            return false;
        }
        if (lhs._hamming != rhs._hamming) {
            return false;
        }
        if (lhs._manhattan != rhs._manhattan) {
            return false;
        }
        if (lhs._inversion != rhs._inversion) {
            return false;
        }
        return lhs._data == rhs._data;
    }

    friend bool operator!=(const Board& lhs, const Board& rhs) { return !(lhs == rhs); }

    friend std::ostream& operator<<(std::ostream& out, const Board& board) { return out << board.to_string(); }
};

#endif  // PUZZLE_BOARD_HPP
