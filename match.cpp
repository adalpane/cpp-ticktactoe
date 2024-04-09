#include <iostream>
#include <string>
#include "types.h"
#include "match.h"

bool Match::makeMove(const Move& move) {
    if (state[move.pos.first + move.pos.second * BOARD_SIZE] !=
        CellState::empty) {
        return false;
    }
    if (move.player != CellState::player1 && move.player != CellState::player2) {
        return false;
    }
    state[move.pos.first + move.pos.second * BOARD_SIZE] = move.player;
    return true;
}

const BoardState& Match::getBoardState() const { return state; }

Outcome Match::getOutcome() {
    if (hasPlayerWon(CellState::player1)) {
        return Outcome::player1Won;
    }
    else if (hasPlayerWon(CellState::player2)) {
        return Outcome::player2Won;
    }
    else if (!isThereACellWithState(CellState::empty)) {
        return Outcome::draw;
    }
    else {
        return Outcome::playerTurn;
    }
}

std::ostream& operator<<(std::ostream& os, const Match& match) {
    for (int i{0}; i < BOARD_SIZE; ++i) {
        os << "|";
        for (int j{0}; j < BOARD_SIZE; ++j) {
            os << match.state[j + i * BOARD_SIZE] << "|";
        }
        os << '\n';
    }
    return os;
}