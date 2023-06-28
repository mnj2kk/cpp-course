#include "puzzle/Board.hpp"

thread_local std::mt19937 Board::_rnd = std::mt19937(std::random_device{}());

std::pair<std::size_t, std::size_t> Board::ceil_pos(const unsigned ceil, const std::size_t size) {
    const std::size_t cnt = size * size;
    const unsigned pos    = (ceil + cnt - 1) % cnt;
    return {pos / size, pos % size};
}

std::size_t Board::diff_abs(std::size_t a, std::size_t b) {
    if (a > b) {
        return a - b;
    }
    return b - a;
}

unsigned Board::hamming(const std::vector<std::vector<unsigned>>& board) {
    unsigned result       = 0;
    const std::size_t cnt = board.size() * board.size();
    for (std::size_t i = 0; i < board.size(); ++i) {
        for (std::size_t j = 0; j < board.size(); ++j) {
            unsigned goal = (i * board.size() + j + 1) % cnt;
            if (board[i][j] != goal) {
                ++result;
            }
        }
    }
    return result;
}
unsigned Board::manhattan(const std::vector<std::vector<unsigned>>& board) {
    unsigned result = 0;
    for (std::size_t i = 0; i < board.size(); ++i) {
        for (std::size_t j = 0; j < board.size(); ++j) {
            if (board[i][j] != 0) {
                const auto [x, y] = ceil_pos(board[i][j], board.size());
                result += Board::diff_abs(x, i) + Board::diff_abs(y, j);
            }
        }
    }
    return result;
}

unsigned Board::inversion(const std::vector<std::vector<unsigned>>& board) {
    unsigned result = 0;
    std::vector<unsigned> fenwick(board.size() * board.size());
    for (std::size_t i = 0; i < board.size(); ++i) {
        for (std::size_t j = 0; j < board.size(); ++j) {
            std::size_t cur_i = board[i][j];
            while (cur_i < fenwick.size()) {
                fenwick[cur_i] += 1;
                cur_i |= cur_i + 1;
            }
            if (board[i][j] == 0) {
                continue;
            }
            unsigned cur = 0;
            cur_i        = board[i][j];
            while (cur_i >= 0) {
                cur += fenwick[cur_i];
                cur_i &= cur_i + 1;
                if (cur_i == 0) {
                    break;
                }
                --cur_i;
            }
            result += i * board.size() + j + 1 - cur;
        }
    }
    return result;
}

std::pair<std::size_t, std::size_t> Board::empty_pos(const std::vector<std::vector<unsigned>>& board) {
    std::size_t i = 0;
    std::size_t j = 0;
    for (; i < board.size(); ++i) {
        bool flag = false;
        for (j = 0; j < board.size(); ++j) {
            if (board[i][j] == 0) {
                flag = true;
                break;
            }
        }
        if (flag) {
            break;
        }
    }
    return {i, j};
}

Board Board::create_goal(unsigned size) {
    std::vector<std::vector<unsigned>> data(size, std::vector<unsigned>(size));
    unsigned cnt = size * size;
    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = 0; j < size; ++j) {
            data[i][j] = (i * size + j + 1) % cnt;
        }
    }
    return {std::move(data)};
}

Board Board::create_random(unsigned size) {
    std::vector<unsigned> array(size * size);
    for (std::size_t i = 0; i < array.size(); ++i) {
        array[i] = (i + 1) % array.size();
    }
    std::shuffle(array.begin(), array.end(), _rnd);
    std::vector<std::vector<unsigned>> data(size, std::vector<unsigned>(size));
    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = 0; j < size; ++j) {
            data[i][j] = array[i * size + j];
        }
    }
    return {std::move(data)};
}

Board::Board() : _data(), _size(), _hamming(), _manhattan(), _inversion(), _empty() {}

Board::Board(const std::vector<std::vector<unsigned>>& data)
    : _data(data)
    , _size(_data.size())
    , _hamming(Board::hamming(_data))
    , _manhattan(Board::manhattan(_data))
    , _inversion(Board::inversion(_data))
    , _empty(Board::empty_pos(_data)) {}

Board::Board(std::vector<std::vector<unsigned>>&& data) noexcept
    : _data(std::forward<std::vector<std::vector<unsigned>>>(data))
    , _size(_data.size())
    , _hamming(Board::hamming(_data))
    , _manhattan(Board::manhattan(_data))
    , _inversion(Board::inversion(_data))
    , _empty(Board::empty_pos(_data)) {}

std::size_t Board::size() const {
    return _size;
}

bool Board::is_goal() const {
    return _hamming == 0;
}

unsigned Board::hamming() const {
    return _hamming;
}

unsigned Board::manhattan() const {
    return _manhattan;
}

std::string Board::to_string() const {
    std::stringstream out;
    for (std::size_t i = 0; i < _data.size(); ++i) {
        for (std::size_t j = 0; j < _data[i].size(); ++j) {
            if (j != 0) {
                out << ' ';
            }
            out << _data[i][j];
        }
        if (i + 1 < _data.size()) {
            out << "\n";
        }
    }
    return out.str();
}

const std::vector<unsigned>& Board::operator[](const std::size_t index) const {
    return _data[index];
}

bool Board::is_solvable() const {
    std::size_t dist = _size - _empty.first;
    if (_size < 2) {
        return true;
    }
    if (_size & 1) {
        return !(_inversion & 1);
    }
    return (_inversion + dist) & 1;
}

Board::Board(const Board& board)
    : _data(board._data)
    , _size(board._size)
    , _hamming(board._hamming)
    , _manhattan(board._manhattan)
    , _inversion(board._inversion)
    , _empty(board._empty) {}

Board::Board(Board&& board) noexcept
    : _data(std::move(board._data))
    , _size(board._size)
    , _hamming(board._hamming)
    , _manhattan(board._manhattan)
    , _inversion(board._inversion)
    , _empty(std::move(board._empty)) {
    board._size      = 0;
    board._hamming   = 0;
    board._manhattan = 0;
    board._inversion = 0;
}

Board& Board::operator=(const Board& board) {
    if (this != &board) {
        _data      = board._data;
        _size      = board._size;
        _hamming   = board._hamming;
        _manhattan = board._manhattan;
        _inversion = board._inversion;
        _empty     = board._empty;
    }
    return *this;
}
Board& Board::operator=(Board&& board) noexcept {
    if (this != &board) {
        _data            = std::move(board._data);
        _size            = board._size;
        _hamming         = board._hamming;
        _manhattan       = board._manhattan;
        _inversion       = board._inversion;
        _empty           = std::move(board._empty);
        board._size      = 0;
        board._hamming   = 0;
        board._manhattan = 0;
        board._inversion = 0;
    }
    return *this;
}
