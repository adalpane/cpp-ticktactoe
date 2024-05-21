#include "./match.h"
#include "variant"


Move strategy1(const Match& match) {
    BoardState board = match.getBoardState();
    Player player  = match.getNext();
    for (int i{2}; i < BOARD_SIZE * BOARD_SIZE; i = i+BOARD_SIZE) {
        if (std::holds_alternative<Blank>(board[i])) {
            return {Coordinates {i % BOARD_SIZE, i / BOARD_SIZE }, player};
        }
    }
    return {};
}