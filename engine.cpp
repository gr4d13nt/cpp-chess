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

int quiescence_search(Board &board, int alpha, int beta)
{
    // Evaluate the position first (stand-pat).
    int stand_pat = heuristic(board);
    Color side = board.sideToMove();

    // ----------------------------
    //  White (maximizing) branch
    // ----------------------------
    if (side == Color::WHITE)
    {
        // Fail-hard beta cutoff check
        // If the stand-pat score is already >= beta,
        // the opponent (Black) won't allow us to reach this position.
        if (stand_pat >= beta)
            return beta;

        // If stand-pat is better than alpha, raise alpha.
        if (stand_pat > alpha)
            alpha = stand_pat;

        // Generate capture moves only
        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

        // Search all captures
        for (const auto &move : captures)
        {
            board.makeMove(move);
            int score = quiescence_search(board, alpha, beta);
            board.unmakeMove(move);

            // If even one capture can force a position >= beta, prune
            if (score >= beta)
                return beta;

            // Update alpha if we found a better (larger) score
            if (score > alpha)
                alpha = score;
        }
        return alpha;
    }

    // ----------------------------
    //  Black (minimizing) branch
    // ----------------------------
    else
    {
        // Fail-hard alpha cutoff check
        // If the stand-pat <= alpha, White won't allow us to reach this position.
        if (stand_pat <= alpha)
            return alpha;

        // If stand-pat is still better (lower) than beta, lower beta.
        if (stand_pat < beta)
            beta = stand_pat;

        // Generate capture moves
        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

        // Search captures
        for (const auto &move : captures)
        {
            board.makeMove(move);
            int score = quiescence_search(board, alpha, beta);
            board.unmakeMove(move);

            // If we can force <= alpha, prune
            if (score <= alpha)
                return alpha;

            // Otherwise update beta to a smaller value
            if (score < beta)
                beta = score;
        }
        return beta;
    }
}

int quiescence_search_badeval(Board &board, int alpha, int beta)
{
    // Evaluate the position first (stand-pat).
    int stand_pat = heuristic_old(board);
    int best_value = stand_pat;
    Color side = board.sideToMove();

    // ----------------------------
    //  White (maximizing) branch
    // ----------------------------
    if (side == Color::WHITE)
    {
        // Fail-hard beta cutoff check
        // If the stand-pat score is already >= beta,
        // the opponent (Black) won't allow us to reach this position.
        if (stand_pat >= beta)
            return stand_pat;

        // If stand-pat is better than alpha, raise alpha.
        if (stand_pat > alpha)
            alpha = stand_pat;

        // Generate capture moves only
        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

        // Search all captures
        for (const auto &move : captures)
        {
            board.makeMove(move);
            int score = quiescence_search_badeval(board, alpha, beta);
            board.unmakeMove(move);

            // If even one capture can force a position >= beta, prune
            if (score >= beta)
                return score;

            if (score > best_value)
                best_value = score;

            // Update alpha if we found a better (larger) score
            if (score > alpha)
                alpha = score;
        }
        return best_value;
    }

    // ----------------------------
    //  Black (minimizing) branch
    // ----------------------------
    else
    {
        // Fail-hard alpha cutoff check
        // If the stand-pat <= alpha, White won't allow us to reach this position.
        if (stand_pat <= alpha)
            return stand_pat;

        // If stand-pat is still better (lower) than beta, lower beta.
        if (stand_pat < beta)
            beta = stand_pat;

        // Generate capture moves
        Movelist captures;
        movegen::legalmoves<movegen::MoveGenType::CAPTURE>(captures, board);

        // Search captures
        for (const auto &move : captures)
        {
            board.makeMove(move);
            int score = quiescence_search_badeval(board, alpha, beta);
            board.unmakeMove(move);

            // If we can force <= alpha, prune
            if (score <= alpha)
                return alpha;

            if (score < best_value)
                best_value = score;

            // Otherwise update beta to a smaller value
            if (score < beta)
                beta = score;
        }
        return best_value;
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

int principal_variation_search_badeval(Board &board, int depth, int alpha, int beta)
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
        return quiescence_search_badeval(board, alpha, beta);
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
                score = principal_variation_search_badeval(board, depth - 1, alpha, beta);
                firstMove = false;
            }
            else
            {
                score = principal_variation_search_badeval(board, depth - 1, alpha, alpha + 1);
                if (score > alpha)
                {
                    score = principal_variation_search_badeval(board, depth - 1, alpha, beta);
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
                score = principal_variation_search_badeval(board, depth - 1, alpha, beta);
                firstMove = false;
            }
            else
            {
                score = principal_variation_search_badeval(board, depth - 1, beta - 1, beta);
                if (score < beta)
                {
                    score = principal_variation_search_badeval(board, depth - 1, alpha, beta);
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

        // Check time limit before starting the next depth
        if (((clock() - start_time) * 1000 / CLOCKS_PER_SEC) >= allotted_time)
        {
            break; // Time's up, return the best move found
        }
    }

    // cout << "pvs nodes searched: " << pvs_nodes_searched << endl;
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
                int score = principal_variation_search_badeval(board, depth, alpha, 10000000);
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
                int score = principal_variation_search_badeval(board, depth, -10000000, beta);
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
