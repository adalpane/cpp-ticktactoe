#include "types.h"
#include "match.h"
#include <random>


Move randomStrategy(const Match& match) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, (BOARD_SIZE * BOARD_SIZE) - 1);
    BoardState board = match.getBoardState();
    while (true) {
        int i = dis(gen);
        if (std::holds_alternative<Blank>(board[i])) {
            return Move{Coordinates {i % BOARD_SIZE, i / BOARD_SIZE}, match.getNext()};
        }
    };
}

using Strategy = Move (*)(Match &);

Outcome simulate(Match &match, Strategy strategy1, Strategy strategy2)
{
    while (std::holds_alternative<Ongoing>(match.getOutcome()))
    {
        match.makeMove(match.getNext() == Player::PlayerOne ? strategy1(match) : strategy2(match));
    }
    return match.getOutcome();
}

int evaluator(Match &match, Move &move)
{
    int count = 0;
    for (int i = 0; i < 200; i++)
    {
        match.makeMove(move);
        Player adversary = match.getNext();
        Outcome outcome = simulate(match, reinterpret_cast<Strategy>(randomStrategy),
                                   reinterpret_cast<Strategy>(randomStrategy));
        if (std::holds_alternative<Won>(outcome))
        {
            return (std::get<Won>(outcome).winner == adversary) ? i : -i;
        }
    }
    // std::cout << count << std::endl;
    return count;
}

Move mcStrategy(Match &match) //, Evaluator evaluator2)
{
    // std::cout << "hey" << std::endl;
    auto moves = match.getAvailableMoves();
    std::vector<std::pair<Move, int>> evaluatedMoves;
    //  per ogni possibile mossa, valutare il risultato della simulazione

    std::transform(moves.begin(), moves.end(),
                   std::back_inserter(evaluatedMoves),
                   [&](Move move)
                   {
                       return std::make_pair(move,
                                             evaluator(match, move));
                   });

    std::sort(evaluatedMoves.begin(), evaluatedMoves.end(),
              [](auto &emove1, auto &emove2)
              {
                  return emove1.second < emove2.second;
              });
    // std::cout << evaluatedMoves.size() << std::endl;
    return evaluatedMoves[0].first;
}
