#include <random>

inline Move strategy2(const BoardState& state, CellState player) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, (BOARD_SIZE * BOARD_SIZE) - 1);

    if (state[4] == CellState::empty){
        return {Point{1, 1}, player};
    }
    else{
        while (true) {
            int i = dis(gen);
            if (state[i] == CellState::empty) {
                return Move{Point{i % BOARD_SIZE, i / BOARD_SIZE}, player};
            }
        };
    }
}