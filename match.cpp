#include <iostream>
#include <string>
#include <vector>
#include "types.h"
#include "match.h"

void Match::makeMove(const Move& move) {
    int index = move.coords.first + move.coords.second * BOARD_SIZE;
    if (std::holds_alternative<Player>(move.player) &&
        next == std::get<Player>(move.player)
        && std::holds_alternative<Blank>(board[index]))
    {
        board[index] = move.player;
        next = next == Player::PlayerOne ? Player::PlayerTwo : Player::PlayerOne;
    }
    else
    {
        throw std::runtime_error("Invalid move");
    }
}

const BoardState& Match::getBoardState() const { return board; }

Outcome Match::getOutcome() {
    if (hasPlayerWon(Player::PlayerOne)) {
        return Won {
            .winner = Player::PlayerOne
        };
    }
    else if (hasPlayerWon(Player::PlayerTwo)) {
        return Won {
            .winner = Player::PlayerOne
        };
    }
    else if (!isThereACellWithState(Blank {})) {
        return Draw{};
    }
    else {
        return Ongoing{};
    }
}

std::ostream& operator<<(std::ostream& os, const Match& match) {
    for (int i{0}; i < BOARD_SIZE; ++i) {
        os << "|";
        for (int j{0}; j < BOARD_SIZE; ++j) {
            os << match.board[j + i * BOARD_SIZE] << "|";
        }
        os << '\n';
    }
    return os;
}

Player Match::getNext() const {
    return next;
}

std::vector<Move> Match::getAvailableMoves() const {
    std::vector<Move> moves;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            int index = i + j * BOARD_SIZE;
            if (std::holds_alternative<Blank>(board[index]))
            {
                moves.push_back(Move{{i, j}, next});
            }
        }
    }
    return moves;
}
