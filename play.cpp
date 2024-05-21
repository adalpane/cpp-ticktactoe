#include "./match.h"
#include "./strategy1.h"

#include "./strategy2.h"

Outcome playGame() {
    Match match{};

    while (true) {
        std::cout << match << '\n';
        Player next = match.getNext();
        Move move = next == Player::PlayerOne ? strategy1(match) : strategy2(match);
        try {
            match.makeMove(move);
            if (!std::holds_alternative<Ongoing>(match.getOutcome())) {
                std::cout << match << '\n';
                return match.getOutcome();
            }
        } catch (std::runtime_error& _){
            std::cout << "Invalid move " << move << '\n';
            return Won { .winner = next == Player::PlayerOne ? Player::PlayerTwo : Player::PlayerOne };
        }
    }
}
