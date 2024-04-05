#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <gsl/gsl>

constexpr int gameSize = 3;
constexpr int gameBoardSize = gameSize * gameSize;

enum cellState
{
    empty,
    player1,
    player2
};

enum gameState
{
    notFinished,
    player1Won,
    player2Won,
    draw
};

using Move = struct
{
    int x;
    int y;
    cellState gs;
};

using CellCoords = std::pair<int, int>;

using Point = std::pair<int, int>;
using Direction = std::pair<int, int>;

using GameState = cellState[gameBoardSize];

class Match
{
    GameState state;

public:
    GameState const &getState() const
    {
        return state;
    }

    void makeMove(Move m)
    {
        Expects(m.gs != empty);
        state[m.y * gameSize + m.x] = m.gs;
    }

    bool isThereTrisAt(Point p, Direction d, cellState cs) const
    {
        for (uint i = 0; i < gameSize; ++i)
        {
            if (getCellContent(p.first + i * d.first, p.second + i * d.second) != cs)
            {
                return false;
            }
        }
        return true;
    }

    gameState checkMatch() const
    {
        std::pair<Point, Direction> trises[] = {
            {{0, 0}, {1, 0}},
            {{0, 1}, {1, 0}},
            {{0, 2}, {1, 0}},
            {{0, 0}, {0, 1}},
            {{1, 0}, {0, 1}},
            {{2, 0}, {0, 1}},
            {{0, 0}, {1, 1}},
            {{2, 2}, {-1, -1}}

        };
        for (auto [p, d] : trises)
        {
            if (isThereTrisAt(p, d, player1))
            {
                return gameState::player1Won;
            }
            if (isThereTrisAt(p, d, player2))
            {
                return gameState::player2Won;
            }
        }
        if (std::any_of(std::begin(state), std::end(state), [](cellState s)
                        { return s == empty; }))
        {
            return gameState::notFinished;
        }
        else
        {
            return gameState::draw;
        }
    }

    cellState getCellContent(int x, int y) const
    {
        return state[x + y * gameSize];
    }

    void setCellContent(int x, int y, cellState c)
    {
        state[x + y * gameSize] = c;
    }

    operator std::string() const
    {
        std::string s{};
        for (int i = 0; i < gameBoardSize; ++i)
        {
            s += std::to_string(getCellContent(i % gameSize, i / gameSize)) +
                 (i % gameSize == 2 ? "\n" : " ");
        }
        return s;
    }

    // constructor with default values empty cells
    Match()
    {
        for (int i = 0; i < gameBoardSize; ++i)
        {
            state[i] = empty;
        }
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