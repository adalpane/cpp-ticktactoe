#include <iostream>
#include <map>
#include <sstream>
#include "./play.h"
#include "match.h"


template <typename KeyType>
class ICounter
{

public:
    virtual void increase(KeyType key) = 0;
    virtual int getCount(KeyType key) = 0;
};

class Counter : public ICounter<Outcome>
{

    std::map<Outcome, int> m_map;

public:
    int getCount(Outcome key) override
    {
        return m_map[key];
    }

    void increase(Outcome key) override
    {
        m_map[key]++;
    }
};

int main(int argc, char *argv[])
{
    std::cout << "Comincia la sfida" << std::endl;
    Match m;

    int n{10};
    if (argc > 1)
    {
        std::stringstream convert{ argv[1] };
        convert >> n;
    }

    Counter counter;

    for (int i = 0; i < n; i++)
    {
        std::cout << "Sfida " << i << ":\n";
        counter.increase(playGame());
    }

    std::cout << "Vinte da 1: " << counter.getCount(Outcome::player1Won) << '\n';
    std::cout << "Vinte da 2: " << counter.getCount(Outcome::player2Won) << std::endl;
}