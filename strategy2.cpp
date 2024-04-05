#include "./match.cpp"

Move strategy2(GameState const &m)
{
    for (int i = 0; i < gameBoardSize; ++i)
    {
        if (m[i] == empty)
        {
            return Move{i % gameSize, i / gameSize, player2};
        }
    }
}