#include "./match.h"
#include "./strategy1.cpp"
#include "./strategy2.cpp"

Outcome playGame() {
    Match m{};

    while (true) {
        std::cout << m << '\n';
        Move m1 = strategy1(m.getBoardState(), CellState::player1);
        if (!m.makeMove(m1)) {
            std::cout << "Player 1: Invalid move " << m1 << '\n';
            return Outcome::player2Won;
        }
        if (m.getOutcome() != Outcome::playerTurn) {
            std::cout << m << '\n';
            return m.getOutcome();
        }

        std::cout << m << '\n';
        Move m2 = strategy2(m.getBoardState(), CellState::player2);
        if (!m.makeMove(m2)) {
            std::cout << "Player 2: Invalid move" << m2 << '\n';
            return Outcome::player1Won;
        }
        if (m.getOutcome() != Outcome::playerTurn) {
            std::cout << m << '\n';
            return m.getOutcome();
        }

        if (m.getOutcome() == Outcome::draw) {
            std::cout << m << '\n';
            return Outcome::draw;
        }
    }
}
