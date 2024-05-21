#ifndef CPP_TICTACTOE_TYPES_H
#define CPP_TICTACTOE_TYPES_H

#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <array>
#include <variant>

enum class Player
{
    PlayerOne,
    PlayerTwo
};

inline std::ostream& operator<<(std::ostream& os, const Player& player) {
    switch (player) {
        case Player::PlayerOne:
            return os << "1";
        case Player::PlayerTwo:
            return os << "2";
    }
    return os;
}
enum class Blank
{
};
struct Ongoing
{
};
struct Draw
{
};
struct Won
{
    Player winner;
};
using Outcome = std::variant<Ongoing, Draw, Won>;
template<>
struct std::hash<Outcome>
{
    std::size_t operator()(const Outcome& o) const noexcept
    {
        if (std::holds_alternative<Draw>(o)) {
            return 0;
        } else if (std::holds_alternative<Won>(o)) {
            switch (std::get<Won>(o).winner) {
                case Player::PlayerOne :  return 1;
                case Player::PlayerTwo :  return 2;
            }
        } else {
            return -1;
        }
    }
};

using CellState = std::variant<Blank, Player>;

constexpr int BOARD_SIZE{3};

inline std::ostream& operator<<(std::ostream& os, const Outcome& outcome) {
    if (std::holds_alternative<Ongoing>(outcome)) {
        return os << "Player turn";
    } else if (std::holds_alternative<Won>(outcome)) {
        Won won = std::get<Won>(outcome);
        return os << "Player " << won.winner << " Won";
    } else if (std::holds_alternative<Draw>(outcome)) {
        return os << "Draw";
    }
    return os;
}

using BoardState = std::array<CellState, BOARD_SIZE * BOARD_SIZE>;
inline std::ostream& operator<<(std::ostream& os, const CellState& cellState) {
    if (std::holds_alternative<Player>(cellState)) {
        switch (std::get<Player>(cellState)) {
            case Player::PlayerOne:
                return os << "x";
            case Player::PlayerTwo:
                return os << "o";
        }
    } else {
        return os << " ";
    }
    return os;
}

using Coordinates = std::pair<int, int>;
using Direction = std::pair<int, int>;

struct PossibleTris {
    Coordinates coords;
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
    Coordinates coords;
    CellState player;
};

inline std::ostream& operator<<(std::ostream& os, const Move& move) {
    return os << "(Coords: " << move.coords.first << ", " << move.coords.second
              << " - Player: " << move.player
              << ")";
}


#endif //CPP_TICTACTOE_TYPES_H
