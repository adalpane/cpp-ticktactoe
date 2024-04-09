#ifndef CPP_TICTACTOE_TYPES_H
#define CPP_TICTACTOE_TYPES_H

#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <array>

constexpr int BOARD_SIZE{3};
enum class Outcome { playerTurn, player1Won, player2Won, draw };

inline std::ostream& operator<<(std::ostream& os, const Outcome& outcome) {
    switch (outcome) {
        case Outcome::playerTurn:
            return os << "Player turn";
        case Outcome::player1Won:
            return os << "Player 1 Won";
        case Outcome::player2Won:
            return os << "Player 2 Won";
        case Outcome::draw:
            return os << "Draw";
    }
    return os;
}

enum class CellState {
    empty,
    player1,
    player2,
};
using BoardState = std::array<CellState, BOARD_SIZE * BOARD_SIZE>;
inline std::ostream& operator<<(std::ostream& os, const CellState& cellState) {
    switch (cellState) {
        case CellState::empty:
            return os << " ";
        case CellState::player1:
            return os << "x";
        case CellState::player2:
            return os << "o";
    }
    return os;
}

using Point = std::pair<int, int>;
using Direction = std::pair<int, int>;

struct PossibleTris {
    Point point;
    Direction direction;
};
constexpr std::array<PossibleTris, 8> possible_tris = {{
       {{0, 0}, {1, 0}},
       {{0, 1}, {1, 0}},
       {{0, 2}, {1, 0}},
       {{0, 0}, {0, 1}},
       {{1, 0}, {0, 1}},
       {{2, 0}, {0, 1}},
       {{0, 0}, {1, 1}},
       {{2, 0}, {-1, 1}},
}};

struct Move {
    Point pos;
    CellState player;
};

inline std::ostream& operator<<(std::ostream& os, const Move& move) {
    return os << "(Position: " << move.pos.first << ", " << move.pos.second
              << " - Player: " << move.player
              << ")";
}


#endif //CPP_TICTACTOE_TYPES_H
