
#include "./match.cpp"
#include <random>

Move strategy1(GameState const &m)
{
    // TODO: implement your strategy here
    // put 1 on the first empty cell
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, gameBoardSize - 1);

    while (true)
    {
        int i = dis(gen);
        if (m[i] == empty)
        {
            return Move{i % gameSize, i / gameSize, player1};
        }
    };
}