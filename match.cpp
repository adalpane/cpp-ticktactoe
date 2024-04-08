#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <array>

constexpr int BOARD_SIZE{3};


enum class CellState {
    empty,
    player1,
    player2,
};

enum class Outcome {
    player1Turn,
    player2Turn,
    player1Won,
    player2Won,
    draw,
};

using Point = std::pair<int, int>;
using Direction = std::pair<int, int>;

struct Move {
    Point pos;
    CellState player;
};


using GameState = CellState[BOARD_SIZE * BOARD_SIZE];


class Match {
private:
    Outcome outcome;
    GameState gameBoard;

    const CellState &getCellState(const Point &pos) const {
        return gameBoard[pos.first + pos.second * BOARD_SIZE];
    }

    void setCellState(const Point &pos, const CellState &player) {
        gameBoard[pos.first + pos.second * BOARD_SIZE] = player;
    }

    bool isThereATris(const Point &p, const Direction &d,
                      const CellState &cellState) {
        for (int i{0}; i < BOARD_SIZE; ++i) {
            const Point pos{p.first + i * d.first, p.second + i * d.second};
            if (getCellState(pos) != cellState) {
                return false;
            }
        }
        return true;
    }

    constexpr bool hasPlayerWon(const CellState &player) {
        constexpr std::array<std::pair<Point, Direction>, 8> combos = {{
               {{0, 0}, {1, 0}},
               {{0, 1}, {1, 0}},
               {{0, 2}, {1, 0}},
               {{0, 0}, {0, 1}},
               {{1, 0}, {0, 1}},
               {{2, 0}, {0, 1}},
               {{0, 0}, {1, 1}},
               {{2, 2}, {-1, -1}},
       }};

        for (const auto &combo : combos) {
            if (isThereATris(combo.first, combo.second, player)) {
                return true;
            }
        }
        return false;
    }

    bool isThereACellWithState(const CellState &state) const {
        return std::any_of(std::begin(gameBoard), std::end(gameBoard),
                           [&state](CellState c) { return c == state; });
    }

public:
    Match() {
        outcome = Outcome::player1Turn;
        for (int i{0}; i < BOARD_SIZE * BOARD_SIZE; ++i) {
            gameBoard[i] = CellState::empty;
        }
    }

    void makeMove(const Move &m) { setCellState(m.pos, m.player); }

    const GameState &getState() const { return gameBoard; }

    Outcome getOutcome() { return outcome; }

    Outcome checkOutcome() {
        /*for (int i{0}; i < BOARD_SIZE * BOARD_SIZE; ++i) {
          if (gameBoard[i] == CellState::empty) {
            return outcome;
          }
        }*/

        /*if (std::any_of(std::begin(gameBoard), std::end(gameBoard),
                        [](CellState c) { return c == CellState::empty; })) {
          return outcome;
        }*/

        if (hasPlayerWon(CellState::player1)) {
            return Outcome::player1Won;
        } else if (hasPlayerWon(CellState::player2)) {
            return Outcome::player2Won;
        } else if (!isThereACellWithState(CellState::empty)) {
            return Outcome::draw;
        } else {
            return outcome;
        }
    }

    operator std::string() const
    {
        std::string s{};
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i)
        {
            const Point pos{i % BOARD_SIZE, i / BOARD_SIZE};
            s += std::to_string((double) getCellState(pos)) +
                 (i % BOARD_SIZE == 2 ? "\n" : " ");
        }
        return s;
    }

    // overload operator << for output streaming
    friend std::ostream &operator<<(std::ostream &os, const Match &m)
    {
        return os << (std::string)m;
    }
};

// int main()
// {
//     std::cout << "ciao a tutti" << std::endl;
//     Match m;
//     m.setCellContent(0, 0, player2);
//     m.setCellContent(0, 1, player1);
//     m.setCellContent(0, 2, player1);
//     m.setCellContent(1, 0, player2);
//     m.setCellContent(1, 1, player1);
//     m.setCellContent(1, 2, player2);
//     m.setCellContent(2, 0, player1);
//     m.setCellContent(2, 1, player2);
//     m.setCellContent(2, 2, player1);
//     std::cout << m.checkMatch() << std::endl;
//     std::cout << m << std::endl;
// }