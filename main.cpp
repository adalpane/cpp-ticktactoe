#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
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

    std::unordered_map<Outcome, int> m_map;

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
        Outcome outcome = playGame();
        std::cout << "Risultato Sfida " << i << ": " << outcome << "\n";
        counter.increase(outcome);
    }

    std::cout << "Vinte da 1: " << counter.getCount(Won { .winner = Player::PlayerOne }) << '\n';
    std::cout << "Vinte da 2: " << counter.getCount(Won { .winner = Player::PlayerTwo }) << std::endl;
}