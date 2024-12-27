#include <random>
#include "chess.hpp"
#include "engine.hpp"

using namespace chess;
using namespace std;


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, 99);

// -1 for black win, 0 for draw, 1 for white win
int play_game(Engine &white, Engine &black)
{
    Board board = Board();
    Move move;

    while (true)
    {
        // cout << board << endl;
        if (board.sideToMove() == Color::WHITE)
        {
            // low chance for random move
            if (dist(gen) < 5)
            {
                Movelist moves;
                movegen::legalmoves(moves, board);
                move = moves[rand() % moves.size()];
            }
            else
            {
                move = white.best_move(board);
            }
        }
        else
        {
            if (dist(gen) < 5)
            {
                Movelist moves;
                movegen::legalmoves(moves, board);
                move = moves[rand() % moves.size()];
            }
            else
            {
                move = black.best_move(board);
            }
        }

        board.makeMove(move);

        if (board.isGameOver().second == GameResult::WIN)
        {
            cout << board << endl;
            if (board.sideToMove() == Color::WHITE)
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        if (board.isGameOver().second == GameResult::DRAW)
        {
            cout << board << endl;
            return 0;
        }
        if (board.isGameOver().second == GameResult::LOSE)
        {
            cout << board << endl;
            if (board.sideToMove() == Color::WHITE)
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }
    }
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

    cout << "P1 wins: " << p1_wins << endl;
    cout << "P2 wins: " << p2_wins << endl;
    cout << "Draws: " << draws << endl;
}

int main()
{
    Engine1 p1 = Engine1(25);
    Engine2 p2 = Engine2(25);

    play_n_games(p1, p2, 14);

    return 0;
}