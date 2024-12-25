#include "chess.hpp"
#include "engine.hpp"

using namespace chess;

// -1 for black win, 0 for draw, 1 for white win
int play_game(Engine &white, Engine &black)
{
    Board board = Board();
    Move move;
    int result = 0;

    if (board.sideToMove() == Color::WHITE) {
        move = white.best_move(board);
    } else {
        move = black.best_move(board);
    }

    board.makeMove(move);

    while (true)
    {
        if (board.isGameOver().second == GameResult::WIN)
        {
            if (board.sideToMove() == Color::WHITE)
            {
                result = 1;
            }
            else
            {
                result = -1;
            }
        }
        if (board.isGameOver().second == GameResult::DRAW)
        {
            result = 0;
        }
        if (board.isGameOver().second == GameResult::LOSE)
        {
            if (board.sideToMove() == Color::WHITE)
            {
                result = -1;
            }
            else
            {
                result = 1;
            }
        }
        if (board.isGameOver().second == GameResult::NONE)
        {
            // print_board(board);
            break;
        }
    }

    return result;
}

void play_n_games(Engine &p1, Engine &p2, int n)
{
    int p1_wins = 0;
    int p2_wins = 0;
    int draws = 0;

    for (int i = 0; i < n / 2; i++)
    {
        int result = play_game(p1, p2);

        if (result == 1)
        {
            p1_wins++;
        }
        if (result == -1)
        {
            p2_wins++;
        }
        if (result == 0)
        {
            draws++;
        }
    }

    for (int i = 0; i < n / 2; i++)
    {
        int result = play_game(p2, p1);

        if (result == 1)
        {
            p2_wins++;
        }
        if (result == -1)
        {
            p1_wins++;
        }
        if (result == 0)
        {
            draws++;
        }
    }

    std::cout << "White wins: " << p1_wins << std::endl;
    std::cout << "Black wins: " << p2_wins << std::endl;
    std::cout << "Draws: " << draws << std::endl;
}

int main()
{
    Engine1 p1 = Engine1(1000);
    Engine2 p2 = Engine2(1000);

    play_n_games(p1, p2, 100);

    return 0;
}