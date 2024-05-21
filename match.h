//
// Created by adriano on 09/04/24.
//

#ifndef CPP_TICTACTOE_MATCH_H
#define CPP_TICTACTOE_MATCH_H

#include "types.h"
#include <ranges>
#include <vector>

class Match {
private:
    Player next {};
    BoardState board{};

    bool isThereATris(const PossibleTris& tris, const CellState& player) {
        for (int i{0}; i < BOARD_SIZE; ++i) {
            const Coordinates pos{tris.coords.first + i * tris.direction.first,
                            tris.coords.second + i * tris.direction.second};
            if (board[pos.first + pos.second * BOARD_SIZE] != player) {
                return false;
            }
        }
        return true;
    }

    bool isThereACellWithState(const CellState& cellState) {
        return std::any_of(std::begin(board), std::end(board), [cellState](CellState cell) { return cell == cellState; });
    }

    bool hasPlayerWon(const CellState& player) {
        return std::any_of(std::begin(possible_tris), std::end(possible_tris), [player, this](PossibleTris tris) { return isThereATris(tris, player); });
    }

public:

    Match() {
        for (auto& cell : board) {
            cell = Blank{};
        }
    }

    void makeMove(const Move& move);

    [[nodiscard]] const BoardState& getBoardState() const;

    Outcome getOutcome();

    friend std::ostream& operator<<(std::ostream& os, const Match& match);

    [[nodiscard]] Player getNext() const;

    [[nodiscard]] std::vector<Move> getAvailableMoves() const;
};
#endif //CPP_TICTACTOE_MATCH_H
