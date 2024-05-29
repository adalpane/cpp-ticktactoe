// https://replit.com/@adrianodalpane/C20

// "https://tinyurl.com/tris-intermedio"

#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>
#include <random>
#include <functional>

constexpr int BOARD_SIZE = 3;

enum class Player { PlayerOne, PlayerTwo };

enum class Blank {};
struct Ongoing {};
struct Draw {};
struct Won {
    Player winner;
};
using Outcome = std::variant<Ongoing, Draw, Won>;

std::ostream &operator<<(std::ostream &cout, Outcome const &o) {
    if (std::holds_alternative<Ongoing>(o)) {
        return cout << "Ongoing";
    } else if (std::holds_alternative<Draw>(o)) {
        return cout << "Draw";
    } else if (std::holds_alternative<Won>(o)) {
        auto w = std::get<Won>(o);
        return cout << "Player " << (static_cast<int>(w.winner) + 1) << " wins";
    }
    return cout;
}

using CellState = std::variant<Blank, Player>;

using Coordinates = std::pair<int, int>;
using Direction = std::pair<int, int>;

Direction operator*(int l, Direction const &d) {
    return Direction{d.first * l, d.second * l};
}

Coordinates operator+(Coordinates const &c, Direction const &d) {
    return Coordinates{c.first + d.first, c.second + d.second};
}

struct Tris {
    Coordinates point;
    Direction direction;
    int length;

    class Iterator {
        Coordinates current;
        Coordinates direction;
        int remaining;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Coordinates;
        using difference_type = std::ptrdiff_t;
        using pointer = Coordinates *;
        using reference = Coordinates &;

        Iterator(Coordinates start, Coordinates dir, int len)
                : current(start), direction(dir), remaining(len) {}

        Coordinates operator*() const { return current; }

        Iterator &operator++() {
            current.first += direction.first;
            current.second += direction.second;
            --remaining;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return remaining != other.remaining;
        }

        bool operator==(const Iterator &other) const {
            return remaining == other.remaining;
        }
    };

    Iterator begin() const { return Iterator{point, direction, length}; }

    Iterator end() const { return Iterator{point, direction, 0}; }
};

struct Move {
    Coordinates coords;
    Player player;
};

using StateView =
        std::pair<std::array<CellState, BOARD_SIZE * BOARD_SIZE>, Player>;

class Board {
    std::array<CellState, BOARD_SIZE * BOARD_SIZE> board;
    Player next;
    int turn;

    std::vector<Tris> possibleTrises() const {
        std::vector<Tris> trises{{{2, 0}, {-1, 1}, 3}, {{0, 0}, {1, 1}, 3}};
        for (int i = 0; i < BOARD_SIZE; i++) {
            trises.push_back({ { i, 0 }, { 0, 1 }, 3});
            trises.push_back({ { 0, i }, { 1, 0 }, 3});
        }
        return trises;
    }


    bool hasTris(Player player) const {
        std::vector<Tris> trises{possibleTrises()};

        auto checkTris = [this, player](const Coordinates & coordinates) {
            auto cell = board[coordinates.first + BOARD_SIZE * coordinates.second];
            return std::holds_alternative<Player>(cell) && std::get<Player>(cell) == player;
        };

        return std::any_of(trises.begin(), trises.end(), [checkTris](auto &tris) {
          return std::all_of(tris.begin(), tris.end(), checkTris);
        });
    }

public:
    Board() { next = Player::PlayerOne; }
    Player getNextPlayer() const { return next; };


    void makeMove(Move const &move) {
        const int index = move.coords.first + move.coords.second * BOARD_SIZE;
        turn++;
        if (next == move.player && std::holds_alternative<Blank>(board[index])) {
            board[index] = move.player;
            next = next == Player::PlayerOne ? Player::PlayerTwo : Player::PlayerOne;
        } else {
            throw std::runtime_error("Invalid move");
        }
    }

    std::vector<Move> getAvailableMoves() const {
        std::vector<Move> moves;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                int index = i + j * BOARD_SIZE;
                if (std::holds_alternative<Blank>(board[index])) {
                    moves.push_back(Move{{i, j}, next});
                }
            }
        }
        return moves;
    }

    Outcome getOutcome() const {
        if (hasTris(Player::PlayerOne)) {
            return Won{.winner = Player::PlayerOne};
        } else if (hasTris(Player::PlayerTwo)) {
            return Won{.winner = Player::PlayerTwo};
        } else if (std::none_of(board.begin(), board.end(), [](auto &cell) {
            return std::holds_alternative<Blank>(cell);
        })) {
            return Draw{};
        }
        return Ongoing{};
    };

    StateView getState(Player player) const {
        return std::make_pair(board, next);
    };

    int getTurn() const {
        return turn;
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};

std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        if (i % BOARD_SIZE == 0) {
            os << std::endl;
        }

        if (std::holds_alternative<Blank>(board.board[i])) {
            os << "-";
        } else {
            if (std::get<Player>(board.board[i]) == Player::PlayerOne) {
                os << "x";
            } else {
                os << "o";
            }
        }
    }
    return os;
};

using Strategy = Move (*)(Board const&);

Move strategy2(const Board &b) {
    Move nextMove;
    auto [brd,nxt] = b.getState(b.getNextPlayer());
    nextMove.player = nxt;

    auto posTris = [&](){
        std::vector<Tris> posTris;
        int len = 3;

        if (std::count_if(brd.begin(), brd.end(), [nxt](auto &cell) {
            return (std::holds_alternative<Player>(cell) && (std::get<Player>(cell) == nxt));
        }) == 1)
        {
            if (std::holds_alternative<Player>(brd[4]) && (std::get<Player>(brd[4]) == nxt))
            {
                if (((std::holds_alternative<Player>(brd[0]) && (std::get<Player>(brd[0]) != nxt)) && (std::holds_alternative<Player>(brd[8]) && (std::get<Player>(brd[8]) != nxt))) ||
                    ((std::holds_alternative<Player>(brd[2]) && (std::get<Player>(brd[2]) != nxt)) && (std::holds_alternative<Player>(brd[6]) && (std::get<Player>(brd[6]) != nxt))))
                {
                    posTris.push_back({{1, 0}, {0, 1}, len});
                    return posTris;
                }
            }
        }

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            posTris.push_back({ { i, 0 }, { 0, 1 }, len});
            posTris.push_back({ { 0, i }, { 1, 0 }, len});
        }
        posTris.push_back({{2, 0}, {-1, 1}, len});
        posTris.push_back({{0, 0}, {1, 1}, len});

        return posTris;
    };

    if (std::holds_alternative<Blank>(brd[4]))
    {
        nextMove.coords = Coordinates{1,1};
    }
    else
    {
        std::vector<Tris> trises{posTris()};
        Coordinates freePos{-1,-1}, tmpPos{-1,-1}, blkPos{-1,-1};

        for(auto &tris : trises)
        {
            int cntMine{}, cntOp{}, cntBlank{};
            for (int i = 0; i < tris.length; i++)
            {
                Coordinates current = tris.point + i * tris.direction;
                auto cell = brd[current.first + current.second * BOARD_SIZE];
                if (std::holds_alternative<Player>(cell))
                {
                    if (std::get<Player>(cell) == nxt)
                    {
                        cntMine++;
                    }
                    else
                    {
                        cntOp++;
                    }
                }
                else if(std::holds_alternative<Blank>(cell))
                {
                    cntBlank++;
                    freePos = current;
                }
            }

            if (cntBlank == 1)
            {
                if (cntMine == 2)
                {
                    nextMove.coords = freePos;
                    return nextMove;
                }
                else if (cntOp == 2)
                {
                    blkPos = freePos;
                }
            }
            else if (cntMine == 1 && cntBlank == 2)
            {
                tmpPos = freePos;
            }
        }

        if (blkPos != Coordinates{-1,-1})
        {
            nextMove.coords = blkPos;
        }
        else if (tmpPos != Coordinates{-1,-1})
        {
            nextMove.coords = tmpPos;
        }
        else
        {
            nextMove.coords = freePos;
        }
    }

    return nextMove;
};

Move strategy1(const Board &b) {

    auto simulate = [](Board &board, std::function<Move(const Board&)> strategy1, std::function<Move(const Board&)> strategy2)
    {
        while (std::holds_alternative<Ongoing>(board.getOutcome()))
        {
            board.makeMove(board.getNextPlayer() == Player::PlayerOne ? strategy1(board) : strategy2(board));
        }
        return std::make_pair(board.getOutcome(), board.getTurn());
    };

    auto evaluator = [simulate](Board const &board, Move &move, std::function<Move(const Board&)> selfStrategy)
    {
        for (int i = 0; i < 100; i++)
        {
            Board tmpBoard = board;
            tmpBoard.makeMove(move);
            Player adversary = tmpBoard.getNextPlayer();
            std::pair<Outcome, int> outcome = simulate(tmpBoard, selfStrategy, selfStrategy);
            if (std::holds_alternative<Won>(outcome.first))
            {
                return (std::get<Won>(outcome.first).winner == adversary) ? -1 : outcome.second;
            } else if (std::holds_alternative<Draw>(outcome.first))
            {
                return 10;
            }
        }
        return -1;
    };

    std::function<Move(const Board&)> mc;

    mc = [evaluator, &mc](const Board& board) {
        auto moves = board.getAvailableMoves();

        //  per ogni possibile mossa, valutare il risultato della simulazione
        if (moves.size() == 1) {
            return moves[0];
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        std::shuffle(std::begin(moves), std::end(moves), gen);

        std::vector<std::pair<Move, int>> evaluatedMoves;


        std::transform(moves.begin(), moves.end(),
                       std::back_inserter(evaluatedMoves),
                       [&](Move move)
                       {
                           int evaluation  = evaluator(board, move, mc);
                           return std::make_pair(move,
                                                 evaluation);
                       });

        std::vector<std::pair<Move, int>> winOrDrawMoves;
        std::copy_if (evaluatedMoves.begin(), evaluatedMoves.end(), std::back_inserter(winOrDrawMoves), [](auto& emove){return emove.second >= 0;} );


        std::sort(winOrDrawMoves.begin(), winOrDrawMoves.end(),
                  [](auto &emove1, auto &emove2)
                  {
                      return emove1.second < emove2.second;
                  });

        //for (auto move : winOrDrawMoves) {
        //    std::cout << static_cast<int>(move.first.player) << ": (" << move.first.coords.first << "," <<  move.first.coords.second << ")=" << move.second << std::endl;
        //}


        if (!winOrDrawMoves.empty()) {
            return winOrDrawMoves[0].first;
        } else {
            return moves[0];
        }
    };
    return mc(b);
};

Outcome playGame(Board& b, Strategy s1, Strategy s2) {
    while (true) {
        std::cout << b << std::endl;

        b.makeMove(s1(b));

        if (!std::holds_alternative<Ongoing>(b.getOutcome())) {
            return b.getOutcome();
        }

        std::cout << b << std::endl;

        b.makeMove(s2(b));

        if (!std::holds_alternative<Ongoing>(b.getOutcome())) {
            return b.getOutcome();
        }

    }
}

int main() {
    Board b{};

    playGame(b, strategy1, strategy2);

    std::cout << b << std::endl;

    std::cout << b.getOutcome();

    return 0;
}