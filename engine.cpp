#include <random>
#include "engine.hpp"
#include "evaluation.hpp"

Engine::Engine(int time)
{
    allotted_time = time;
    start_time = clock();
}

Move Engine::best_move(Board &board)
{
    return -1;
}

Move Engine1::best_move(Board &board)
{
    Movelist moves;
    movegen::legalmoves(moves, board);

    if (moves.size() == 0)
    {
        return -1;
    }
    int best_move = rand() % moves.size();

    return moves[best_move];
}

Move Engine2::best_move(Board &board)
{
    Movelist moves;
    movegen::legalmoves(moves, board);

    // return the move that gives the best heuristic score
    if (board.sideToMove() == Color::WHITE)
    {
        int best_score = -1000000;
        Move best_move = -1;

        for (const auto &move : moves)
        {
            board.makeMove(move);
            int score = heuristic(board);
            board.unmakeMove(move);

            if (score > best_score)
            {
                best_score = score;
                best_move = move;
            }
        }

        return best_move;
    }
    else
    {
        int best_score = 1000000;
        Move best_move = -1;

        for (const auto &move : moves)
        {
            board.makeMove(move);
            int score = heuristic(board);
            board.unmakeMove(move);

            if (score < best_score)
            {
                best_score = score;
                best_move = move;
            }
        }

        return best_move;
    }
}