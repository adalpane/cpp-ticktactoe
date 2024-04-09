//
// Created by adriano on 09/04/24.
//

#ifndef CPP_TICTACTOE_MATCH_H
#define CPP_TICTACTOE_MATCH_H

#include "types.h"
#include <ranges>

class Match {
private:
    BoardState state{};

    bool isThereATris(const PossibleTris& tris, const CellState& player) {
        for (int i{0}; i < BOARD_SIZE; ++i) {
            const Point pos{tris.point.first + i * tris.direction.first,
                            tris.point.second + i * tris.direction.second};
            if (state[pos.first + pos.second * BOARD_SIZE] != player) {
                return false;
            }
        }
        return true;
    }

    bool isThereACellWithState(const CellState& cellState) {
        return std::any_of(std::begin(state), std::end(state), [cellState](CellState cell) { return cell == cellState; });
    }

    bool hasPlayerWon(const CellState& player) {
        return std::any_of(std::begin(possible_tris), std::end(possible_tris), [player, this](PossibleTris tris) { return isThereATris(tris, player); });
    }

public:

    Match() {
        for (auto& cell : state) {
            cell = CellState::empty;
        }
    }

    bool makeMove(const Move& move);

    [[nodiscard]] const BoardState& getBoardState() const;

    Outcome getOutcome();

    friend std::ostream& operator<<(std::ostream& os, const Match& match);
};
#endif //CPP_TICTACTOE_MATCH_H
