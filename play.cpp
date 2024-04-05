#include "./match.cpp"
#include "./strategy1.cpp"
#include "./strategy2.cpp"

auto playMatch()
{
    Match m;
    while (true)
    {
        m.makeMove(strategy1(m.getState()));
        if (m.checkMatch() != notFinished)
        {
            return m.checkMatch();
        }
        m.makeMove(strategy2(m.getState()));
        if (m.checkMatch() != notFinished)
        {
            return m.checkMatch();
        }
    }
}

int count(std::vector<gameState> const &v, gameState gs)
{
    int count = 0;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == gs)
        {
            count++;
        }
    }

    return count;
}

template <typename KeyType>
class ICounter
{

public:
    virtual void increase(KeyType key) = 0;
    virtual int getCount(KeyType key) = 0;
};

class Counter : public ICounter<gameState>
{
    struct Pair
    {
        gameState key;
        uint value;
    };

    std::vector<Pair> v;

public:
    int getCount(gameState key)
    {
        for (int i = 0; i < v.size(); i++)
        {
            if (v[i].key == key)
            {
                return v[i].value;
            }
        }
    }

    void increase(gameState key)
    {
        for (int i = 0; i < v.size(); i++)
        {
            if (v[i].key == key)
            {
                v[i].value++;
                return;
            }
        }
        v.push_back({key, 1});
    }
};

int main(int argc, char *argv[])
{
    std::cout << "Comincia la sfida" << std::endl;
    Match m;

    int n{10};
    if (argc > 1)
    {
        n = std::atoi(argv[1]);
    }

    Counter counter;

    for (int i = 0; i < n; i++)
    {
        // results[i] = playMatch();
        counter.increase(playMatch());
        // std::cout << "match " << i << ": " << results[i] << std::endl;
    }

    std::cout << "vinte da 1: " << counter.getCount(player1Won) << std::endl;
    std::cout << "vinte da 2: " << counter.getCount(player2Won) << std::endl;
}