#include "./match.h"


inline Move strategy1(const BoardState& state, CellState player) {
    for (int i{2}; i < BOARD_SIZE * BOARD_SIZE; i = i+BOARD_SIZE) {
        if (state[i] == CellState::empty) {
            return {Point{i % BOARD_SIZE, i / BOARD_SIZE }, player};
        }
    }
    return {};
}