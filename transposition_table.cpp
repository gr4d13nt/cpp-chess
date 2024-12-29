#include "chess.hpp"

using namespace chess;
using namespace std;

#define SIZE 0x400000

enum Flag
{
    EXACT,
    LOWERBOUND,
    UPPERBOUND
};

struct Entry
{
    uint64_t key;
    int depth;
    int flag;
    int score;
    Move best_move;
};

Entry table[SIZE];

int check_for_entry(uint64_t key, int depth, int alpha, int beta)
{
    Entry *entry = &table[key % SIZE];

    if (entry->key == key)
    {
        if (entry->depth >= depth)
        {
            if (entry->flag == EXACT)
            {
                return entry->score;
            }
            if (entry->flag == LOWERBOUND)
            {
                if (entry->score >= beta)
                {
                    return entry->score;
                }
            }
            if (entry->flag == UPPERBOUND)
            {
                if (entry->score <= alpha)
                {
                    return entry->score;
                }
            }
        }
        // remember_best_move();
    }
    return -1;
}

void store_entry(uint64_t key, int depth, int flag, int score, Move best_move)
{
    Entry *entry = &table[key % SIZE];
    entry->key = key;
    entry->depth = depth;
    entry->flag = flag;
    entry->score = score;
    entry->best_move = best_move;
}
