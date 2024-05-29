// https://replit.com/@adrianodalpane/C20
#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>
#include <random>
#include <functional>

constexpr int BOARD_SIZE = 3;

enum class Player { PlayerOne, PlayerTwo };

enum class Blank {};
struct Ongoing {};
struct Draw {};
struct Won {
    Player winner;
};
using Outcome = std::variant<Ongoing, Draw, Won>;

std::ostream &operator<<(std::ostream &cout, Outcome const &o) {
    if (std::holds_alternative<Ongoing>(o)) {
        return cout << "Ongoing";
    } else if (std::holds_alternative<Draw>(o)) {
        return cout << "Draw";
    } else if (std::holds_alternative<Won>(o)) {
        auto w = std::get<Won>(o);
        return cout << "Player " << (static_cast<int>(w.winner) + 1) << " wins";
    }
    return cout;
}

using CellState = std::variant<Blank, Player>;

using Coordinates = std::pair<int, int>;
using Direction = std::pair<int, int>;

Direction operator*(int l, Direction const &d) {
    return Direction{d.first * l, d.second * l};
}

Coordinates operator+(Coordinates const &c, Direction const &d) {
    return Coordinates{c.first + d.first, c.second + d.second};
}

struct Tris {
    Coordinates point;
    Direction direction;
    int length;

    class Iterator {
        Coordinates current;
        Coordinates direction;
        int remaining;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Coordinates;
        using difference_type = std::ptrdiff_t;
        using pointer = Coordinates *;
        using reference = Coordinates &;

        Iterator(Coordinates start, Coordinates dir, int len)
                : current(start), direction(dir), remaining(len) {}

        Coordinates operator*() const { return current; }

        Iterator &operator++() {
            current.first += direction.first;
            current.second += direction.second;
            --remaining;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return remaining != other.remaining;
        }

        bool operator==(const Iterator &other) const {
            return remaining == other.remaining;
        }

        bool isValid() const {
            return current.first >= 0 && current.first < BOARD_SIZE &&
                   current.second >= 0 && current.second < BOARD_SIZE;
        }
    };

    Iterator begin() const { return Iterator{point, direction, length}; }

    Iterator end() const { return Iterator{point, direction, 0}; }
};

struct Move {
    Coordinates coords;
    Player player;
};

using StateView =
        std::pair<std::array<CellState, BOARD_SIZE * BOARD_SIZE>, Player>;

class Board {
    std::array<CellState, BOARD_SIZE * BOARD_SIZE> board;
    Player next;
    int turn;

    std::vector<Tris> possibleTrises() const {
        std::vector<Tris> trises{{{2, 0}, {-1, 1}, 3}, {{0, 0}, {1, 1}, 3}};
        for (int i = 0; i < BOARD_SIZE; i++) {
            trises.push_back({ { i, 0 }, { 0, 1 }, 3});
            trises.push_back({ { 0, i }, { 1, 0 }, 3});
        }
        return trises;
    }

    bool hasTris(Player player) const {
        std::vector<Tris> trises{possibleTrises()};
        Board const *b = this;
        return std::any_of(
                trises.begin(), trises.end(), [player, b](const Tris &tris) {
                    return std::all_of(tris.begin(), tris.end(),
                                       [player, b](const Coordinates &coord) {
                                           CellState cell = b->getCell(coord);
                                           return std::holds_alternative<Player>(cell) &&
                                                  std::get<Player>(cell) == player;
                                       });
                });
    }

public:
    Board() { next = Player::PlayerOne; }
    Player getNextPlayer() const { return next; };

    CellState getCell(Coordinates c) const {
        return board[c.first + c.second * BOARD_SIZE];
    }


    void makeMove(Move const &move) {
        const int index = move.coords.first + move.coords.second * BOARD_SIZE;
        turn++;
        if (next == move.player && std::holds_alternative<Blank>(board[index])) {
            board[index] = move.player;
            next = next == Player::PlayerOne ? Player::PlayerTwo : Player::PlayerOne;
        } else {
            throw std::runtime_error("Invalid move");
        }
    }

    std::vector<Move> getAvailableMoves() const {
        std::vector<Move> moves;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                int index = i + j * BOARD_SIZE;
                if (std::holds_alternative<Blank>(board[index])) {
                    moves.push_back(Move{{i, j}, next});
                }
            }
        }
        return moves;
    }

    Outcome getOutcome() const {
        if (hasTris(Player::PlayerOne)) {
            return Won{.winner = Player::PlayerOne};
        } else if (hasTris(Player::PlayerTwo)) {
            return Won{.winner = Player::PlayerTwo};
        } else if (std::none_of(board.begin(), board.end(), [](auto &cell) {
            return std::holds_alternative<Blank>(cell);
        })) {
            return Draw{};
        }
        return Ongoing{};
    };

    StateView getState(Player player) const {
        return std::make_pair(board, next);
    };

    int getTurn() const {
        return turn;
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};

std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (i % BOARD_SIZE == 0) {
            os << std::endl;
        }

        if (std::holds_alternative<Blank>(board.board[i])) {
            os << "-";
        } else {
            if (std::get<Player>(board.board[i]) == Player::PlayerOne) {
                os << "x";
            } else {
                os << "o";
            }
        }
    }
    return os;
};

using Strategy = Move (*)(Board const&);

Move strategy1(const Board &b) {
    ${strategy_body1}
}

Move strategy2(const Board &b) {
    ${strategy_body2}
}


Outcome playGame(Board& b, Strategy s1, Strategy s2) {
    while (true) {
        std::cout << b << std::endl;

        b.makeMove(s1(b));

        if (!std::holds_alternative<Ongoing>(b.getOutcome())) {
            return b.getOutcome();
        }

        std::cout << b << std::endl;

        b.makeMove(s2(b));

        if (!std::holds_alternative<Ongoing>(b.getOutcome())) {
            return b.getOutcome();
        }
    }
}

int main() {
    Board b{};

    playGame(b, strategy1, strategy2);

    std::cout << b << std::endl;

    std::cout << b.getOutcome();

    return 0;
}