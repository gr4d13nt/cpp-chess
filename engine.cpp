#include <random>
#include <vector>
#include "engine.hpp"
#include "evaluation.hpp"

Engine::Engine(int time)
{
    allotted_time = time;
    start_time = clock();
}

Move Engine::best_move(Board &board)
{
    return 0;
}

int quiescence_search(Board &board, int alpha, int beta)
{
    int stand_pat = heuristic(board);
    Color side = board.sideToMove();

    if (side == Color::WHITE)
    {
        if (stand_pat >= beta)
            return beta;

        if (stand_pat > alpha)
            alpha = stand_pat;

        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

        // search all captures
        for (const auto &move : captures)
        {
            board.makeMove(move);
            int score = quiescence_search(board, alpha, beta);
            board.unmakeMove(move);

            if (score >= beta)
                return beta;

            if (score > alpha)
                alpha = score;
        }

        return alpha;
    }

    else
    {
        if (stand_pat <= alpha)
            return alpha;

        if (stand_pat < beta)
            beta = stand_pat;

        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

        // search captures
        for (const auto &move : captures)
        {
            board.makeMove(move);
            int score = quiescence_search(board, alpha, beta);
            board.unmakeMove(move);

            if (score <= alpha)
                return alpha;

            if (score < beta)
                beta = score;
        }

        return beta;
    }
}

int principal_variation_search(Board &board, int depth, int alpha, int beta)
{
    if (board.isGameOver().first != GameResultReason::NONE)
    {
        auto result = board.isGameOver().second;

        if (result == GameResult::WIN)
        {
            return board.sideToMove() == Color::WHITE ? 1000000 : -1000000;
        }
        else if (result == GameResult::DRAW)
        {
            return 0;
        }
    }
    if (depth == 0)
    {
        return quiescence_search(board, alpha, beta);
    }

    Movelist moves;
    movegen::legalmoves(moves, board);
    Color side = board.sideToMove();

    bool firstMove = true;
    int score;

    if (side == Color::WHITE)
    {
        int a = -10000000;
        for (Move m : moves)
        {
            board.makeMove(m);
            if (firstMove)
            {
                score = principal_variation_search(board, depth - 1, alpha, beta);
                firstMove = false;
            }
            else
            {
                score = principal_variation_search(board, depth - 1, alpha, alpha + 1);
                if (score > alpha)
                {
                    score = principal_variation_search(board, depth - 1, alpha, beta);
                }
            }
            board.unmakeMove(m);

            a = max(a, score);
            alpha = max(alpha, a);
            if (alpha >= beta)
            {
                break;
            }
        }
        return a;
    }
    else
    {
        int b = 10000000;
        for (Move m : moves)
        {
            board.makeMove(m);
            if (firstMove)
            {
                score = principal_variation_search(board, depth - 1, alpha, beta);
                firstMove = false;
            }
            else
            {
                score = principal_variation_search(board, depth - 1, beta - 1, beta);
                if (score < beta)
                {
                    score = principal_variation_search(board, depth - 1, alpha, beta);
                }
            }
            board.unmakeMove(m);

            b = min(b, score);
            beta = min(beta, b);
            if (alpha >= beta)
            {
                break;
            }
        }
        return b;
    }
}

Move Engine1::best_move(Board &board)
{
    Movelist moves;
    movegen::legalmoves(moves, board);

    if (moves.empty())
    {
        throw std::runtime_error("No legal moves available.");
    }

    Move best_move;
    Move prev_best_move;
    int best_score = board.sideToMove() == Color::WHITE ? -1000000 : 1000000;
    int depth = 1;
    Color side = board.sideToMove();
    start_time = clock();

    // iterative deepening loop
    while (true)
    {
        if (depth > 1)
        {
            auto it = find(moves.begin(), moves.end(), prev_best_move);
            if (it != moves.end())
            {
                iter_swap(moves.begin(), it); // move prev_best_move to the front
            }
        }

        Move current_best_move = moves[0];
        if (side == Color::WHITE)
        {
            int alpha = -10000000;
            for (Move m : moves)
            {
                board.makeMove(m);
                int score = principal_variation_search(board, depth, alpha, 10000000);
                board.unmakeMove(m);
                if (score > alpha)
                {
                    alpha = score;
                    current_best_move = m;
                }
            }
        }
        else
        {
            int beta = 10000000;
            for (Move m : moves)
            {
                board.makeMove(m);
                int score = principal_variation_search(board, depth, -10000000, beta);
                board.unmakeMove(m);
                if (score < beta)
                {
                    beta = score;
                    current_best_move = m;
                }
            }
        }

        best_move = current_best_move;
        prev_best_move = best_move;
        depth++;

        // check time limit before starting the next depth
        if (((clock() - start_time) * 1000 / CLOCKS_PER_SEC) >= allotted_time)
        {
            break;
        }
    }

    return best_move;
}

Move Engine2::best_move(Board &board)
{
    Movelist moves;
    movegen::legalmoves(moves, board);

    if (moves.empty())
    {
        throw std::runtime_error("No legal moves available.");
    }

    Move best_move;
    Move prev_best_move;
    int best_score = board.sideToMove() == Color::WHITE ? -1000000 : 1000000;
    int depth = 1;
    Color side = board.sideToMove();
    start_time = clock();

    // iterative deepening loop
    while (true)
    {
        if (depth > 1)
        {
            auto it = find(moves.begin(), moves.end(), prev_best_move);
            if (it != moves.end())
            {
                iter_swap(moves.begin(), it); // move prev_best_move to the front
            }
        }

        Move current_best_move = moves[0];
        if (side == Color::WHITE)
        {
            int alpha = -10000000;
            for (Move m : moves)
            {
                board.makeMove(m);
                int score = principal_variation_search(board, depth, alpha, 10000000);
                board.unmakeMove(m);
                if (score > alpha)
                {
                    alpha = score;
                    current_best_move = m;
                }
            }
        }
        else
        {
            int beta = 10000000;
            for (Move m : moves)
            {
                board.makeMove(m);
                int score = principal_variation_search(board, depth, -10000000, beta);
                board.unmakeMove(m);
                if (score < beta)
                {
                    beta = score;
                    current_best_move = m;
                }
            }
        }

        best_move = current_best_move;
        prev_best_move = best_move;
        depth++;

        // check time limit before starting the next depth
        if (((clock() - start_time) * 1000 / CLOCKS_PER_SEC) >= allotted_time)
        {
            break;
        }
    }

    return best_move;
}
