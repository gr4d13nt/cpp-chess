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

int minimax(Board &board, int depth)
{
    if (depth == 0 || board.isGameOver().first != GameResultReason::NONE) {
        auto result = board.isGameOver().second;

        if (result == GameResult::WIN) {
            return board.sideToMove() == Color::WHITE ? 1000000 : -1000000;
        } else if (result == GameResult::DRAW) {
            return 0; // Draw
        }
        return heuristic(board);
    }

    Movelist moves;
    movegen::legalmoves(moves, board);

    if (board.sideToMove() == Color::WHITE)
    {
        int best_score = -1000000;

        for (const auto &move : moves)
        {
            board.makeMove(move);
            int score = minimax(board, depth - 1);
            board.unmakeMove(move);

            if (score > best_score)
            {
                best_score = score;
            }
        }

        return best_score;
    }
    else
    {
        int best_score = 1000000;

        for (const auto &move : moves)
        {
            board.makeMove(move);
            int score = minimax(board, depth - 1);
            board.unmakeMove(move);

            if (score < best_score)
            {
                best_score = score;
            }
        }

        return best_score;
    }
}

Move Engine2::best_move(Board &board) {
    Movelist moves;
    movegen::legalmoves(moves, board);

    if (moves.empty()) {
        throw std::runtime_error("No legal moves available.");
    }

    int best_score = board.sideToMove() == Color::WHITE ? -1000000 : 1000000;
    Move best_move;

    for (const auto &move : moves) {
        board.makeMove(move);
        int score = minimax(board, 2);
        board.unmakeMove(move);

        if ((board.sideToMove() == Color::WHITE && score > best_score) ||
            (board.sideToMove() == Color::BLACK && score < best_score)) {
            best_score = score;
            best_move = move;
        }
    }

    return best_move;
}
