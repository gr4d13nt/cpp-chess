#include "chess.hpp"

using namespace chess;

class Engine
{
private:
    int allotted_time; // in milliseconds
    clock_t start_time;

public:
    Engine(int time);

    virtual Move best_move(Board &board);
};

class Engine1 : public Engine
{
public:
    Engine1(int time) : Engine(time) {}
    Move best_move(Board &board) override;
};

class Engine2 : public Engine
{
public:
    Engine2(int time) : Engine(time) {}
    Move best_move(Board &board) override;
};