#include <random>
#include "types.h"
#include "match.h"

Move strategy2(const Match& match) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, (BOARD_SIZE * BOARD_SIZE) - 1);

    BoardState board = match.getBoardState();
    Player player = match.getNext();

    if (std::holds_alternative<Blank>(board[4])){
        return {Coordinates {1, 1}, player};
    }
    else{
        while (true) {
            int i = dis(gen);
            if (std::holds_alternative<Blank>(board[i])) {
                return Move{Coordinates {i % BOARD_SIZE, i / BOARD_SIZE}, player};
            }
        };
    }
}