#include <random>
#include <vector>
#include "engine.hpp"
#include "evaluation.hpp"

int pvs_nodes_searched = 0;
int minimax_nodes_searched = 0;

Engine::Engine(int time)
{
    allotted_time = time;
    start_time = clock();
}

Move Engine::best_move(Board &board)
{
    return -1;
}

int minimax_alphabeta(Board &board, int depth, int alpha, int beta)
{

    minimax_nodes_searched++;
    // Base case: depth is 0 or game is over
    if (depth == 0 || board.isGameOver().first != GameResultReason::NONE)
    {
        auto result = board.isGameOver().second;

        if (result == GameResult::WIN)
        {
            return board.sideToMove() == Color::WHITE ? 1000000 : -1000000;
        }
        else if (result == GameResult::DRAW)
        {
            return 0; // Draw
        }
        return heuristic(board);
    }

    Movelist moves;
    movegen::legalmoves(moves, board);
    Color side = board.sideToMove();

    if (side == Color::WHITE)
    {
        int a = -10000000;
        for (Move m : moves)
        {
            board.makeMove(m);
            a = max(a, minimax_alphabeta(board, depth - 1, alpha, beta));
            board.unmakeMove(m);
            alpha = max(alpha, a);
            if (alpha >= beta)
            {
                break;
            }
        }
        if (a > 100000)
        {
            return a - 500;
        }
        return a;
    }
    else
    {
        int b = 10000000;
        for (Move m : moves)
        {
            board.makeMove(m);
            b = min(b, minimax_alphabeta(board, depth - 1, alpha, beta));
            board.unmakeMove(m);
            beta = min(beta, b);
            if (alpha >= beta)
            {
                break;
            }
        }
        if (b < -100000)
        {
            return b + 500;
        }
        return b;
    }
}

int principal_variation_search(Board &board, int depth, int alpha, int beta)
{
    pvs_nodes_searched++;
    // Base case: depth is 0 or game is over
    if (board.isGameOver().first != GameResultReason::NONE)
    {
        auto result = board.isGameOver().second;

        if (result == GameResult::WIN)
        {
            return board.sideToMove() == Color::WHITE ? 1000000 : -1000000;
        }
        else if (result == GameResult::DRAW)
        {
            return 0; // Draw
        }
    }
    if (depth == 0)
    {
        return heuristic(board);
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
    pvs_nodes_searched = 0;

    if (moves.empty())
    {
        throw std::runtime_error("No legal moves available.");
    }

    // Basic setup
    start_time = clock();
    Color side = board.sideToMove();
    Move best_move, prev_best_move;
    int depth = 1;
    const int INF = 10000000;

    // Iterative deepening
    while (true)
    {
        // Move the best move from the previous iteration to the front
        if (depth > 1)
        {
            auto it = std::find(moves.begin(), moves.end(), prev_best_move);
            if (it != moves.end())
            {
                std::iter_swap(moves.begin(), it);
            }
        }

        // Initialize alpha/beta; pick a sentinel “bestValue”
        // (maximizing if White, minimizing if Black)
        int alpha = -INF;
        int beta = INF;
        int bestValue = (side == Color::WHITE ? alpha : beta);
        Move current_best = moves.front();

        // Search each move
        for (auto &m : moves)
        {
            board.makeMove(m);
            int score = principal_variation_search(board, depth, alpha, beta);
            board.unmakeMove(m);

            if (side == Color::WHITE)
            {
                // White maximizes
                if (score > bestValue)
                {
                    bestValue = score;
                    current_best = m;
                    alpha = std::max(alpha, bestValue);
                    if (alpha >= beta)
                        break; // pruning
                }
            }
            else
            {
                // Black minimizes
                if (score < bestValue)
                {
                    bestValue = score;
                    current_best = m;
                    beta = std::min(beta, bestValue);
                    if (alpha >= beta)
                        break; // pruning
                }
            }
        }

        // Update the global best move and depth
        best_move = current_best;
        prev_best_move = best_move;
        depth++;

        // Check the allotted time
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
    pvs_nodes_searched = 0;

    if (moves.empty())
    {
        throw std::runtime_error("No legal moves available.");
    }

    Move best_move;
    Move prev_best_move;
    int best_score = board.sideToMove() == Color::WHITE ? -1000000 : 1000000;

    int depth = 1; // Start with depth 1
    Color side = board.sideToMove();

    // Record the start time
    start_time = clock();

    // Iterative deepening loop
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

        // Update the best move found so far

        // best_score = current_best_score;
        best_move = current_best_move;
        prev_best_move = best_move;

        // Increment depth for the next iteration
        depth++;
        // cout << "pvs: " << depth << endl;

        // Check time limit before starting the next depth
        if (((clock() - start_time) * 1000 / CLOCKS_PER_SEC) >= allotted_time)
        {
            break; // Time's up, return the best move found
        }
    }

    // cout << "pvs nodes searched: " << pvs_nodes_searched << endl;
    return best_move;
}
