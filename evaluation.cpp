#include "evaluation.hpp"

int piece_square_tables[14][64] = {
    // White pawn
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5, 5, 10, 25, 25, 10, 5, 5,
        0, 0, 0, 20, 20, 0, 0, 0,
        5, -5, -10, 0, 0, -10, -5, 5,
        5, 10, 10, -20, -20, 10, 10, 5,
        0, 0, 0, 0, 0, 0, 0, 0},
    // White knight
    {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50},
    // White bishop
    {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 10, 10, 5, 0, -10,
        -10, 5, 5, 10, 10, 5, 5, -10,
        -10, 0, 10, 10, 10, 10, 0, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -20, -10, -10, -10, -10, -10, -10, -20},
    // White rook
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 10, 10, 10, 10, 10, 10, 5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        0, 0, 0, 5, 5, 0, 0, 0},
    // White queen
    {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 5, 5, 5, 0, -5,
        0, 0, 5, 5, 5, 5, 0, -5,
        -10, 5, 5, 5, 5, 5, 0, -10,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20},
    // White king middle game
    {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20, 20, 0, 0, 0, 0, 20, 20,
        20, 30, 10, 0, 0, 10, 30, 20},
    // White king endgame
    {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -30, -20, -10, 0, 0, -10, -20, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -30, 0, 0, 0, 0, -30, -30,
        -50, -30, -30, -30, -30, -30, -30, -50},
    {0, 0, 0, 0, 0, 0, 0, 0,
     5, 10, 10, -20, -20, 10, 10, 5,
     5, -5, -10, 0, 0, -10, -5, 5,
     0, 0, 0, 20, 20, 0, 0, 0,
     5, 5, 10, 25, 25, 10, 5, 5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
     0, 0, 0, 0, 0, 0, 0, 0},
    // Black knight (mirrored from White knight)
    {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50},
    // Black bishop (mirrored from White bishop)
    {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 0, 10, 10, 10, 10, 0, -10,
        -10, 5, 5, 10, 10, 5, 5, -10,
        -10, 0, 5, 10, 10, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -10, -10, -10, -10, -20},
    // Black rook (mirrored from White rook)
    {
        0, 0, 0, 5, 5, 0, 0, 0,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        5, 10, 10, 10, 10, 10, 10, 5,
        0, 0, 0, 0, 0, 0, 0, 0},
    // Black queen (mirrored from White queen)
    {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -10, 5, 5, 5, 5, 5, -10,
        -5, 0, 5, 5, 5, 5, 0, -5,
        0, 0, 5, 5, 5, 5, 0, -5,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20},
    // Black king middle game (mirrored from White king middle game)
    {
        20, 30, 10, 0, 0, 10, 30, 20,
        20, 20, 0, 0, 0, 0, 20, 20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30},
    // Black king endgame (mirrored from White king endgame)
    {
        -50, -30, -30, -30, -30, -30, -30, -50,
        -30, -30, 0, 0, 0, 0, -30, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -20, -10, 0, 0, -10, -20, -30,
        -50, -40, -30, -20, -20, -30, -40, -50}};

// TODO: tapered eval and improve piece square tables, perhaps using Texel's tuning method
int heuristic(Board &board)
{
    int score = 0;
    for (Square sq = Square::underlying::SQ_A1; sq <= Square::underlying::SQ_H8; ++sq)
    {
        if (board.at(sq) == Piece::WHITEPAWN)
        {
            score += (100 + piece_square_tables[0][sq.index()]);
        }
        if (board.at(sq) == Piece::WHITEKNIGHT)
        {
            score += 300 + piece_square_tables[1][sq.index()];
        }
        if (board.at(sq) == Piece::WHITEBISHOP)
        {
            score += 300 + piece_square_tables[2][sq.index()];
        }
        if (board.at(sq) == Piece::WHITEROOK)
        {
            score += 500 + piece_square_tables[3][sq.index()];
        }
        if (board.at(sq) == Piece::WHITEQUEEN)
        {
            score += 900 + piece_square_tables[4][sq.index()];
        }
        if (board.at(sq) == Piece::WHITEKING)
        {
            score += 10000 + piece_square_tables[5][sq.index()];
        }
        if (board.at(sq) == Piece::BLACKPAWN)
        {
            score -= 100 + piece_square_tables[7][sq.index()];
        }
        if (board.at(sq) == Piece::BLACKKNIGHT)
        {
            score -= 300 + piece_square_tables[8][sq.index()];
        }
        if (board.at(sq) == Piece::BLACKBISHOP)
        {
            score -= 300 + piece_square_tables[9][sq.index()];
        }
        if (board.at(sq) == Piece::BLACKROOK)
        {
            score -= 500 + piece_square_tables[10][sq.index()];
        }
        if (board.at(sq) == Piece::BLACKQUEEN)
        {
            score -= 900 + piece_square_tables[11][sq.index()];
        }
        if (board.at(sq) == Piece::BLACKKING)
        {
            score -= 10000 + piece_square_tables[12][sq.index()];
        }
    }

    return score;
}

int heuristic_old(Board &board)
{
    int score = 0;
    for (Square sq = Square::underlying::SQ_A1; sq <= Square::underlying::SQ_H8; ++sq)
    {
        if (board.at(sq) == Piece::WHITEPAWN)
        {
            score += 100;
        }
        if (board.at(sq) == Piece::WHITEKNIGHT)
        {
            score += 300;
        }
        if (board.at(sq) == Piece::WHITEBISHOP)
        {
            score += 300;
        }
        if (board.at(sq) == Piece::WHITEROOK)
        {
            score += 500;
        }
        if (board.at(sq) == Piece::WHITEQUEEN)
        {
            score += 900;
        }
        if (board.at(sq) == Piece::WHITEKING)
        {
            score += 10000;
        }
        if (board.at(sq) == Piece::BLACKPAWN)
        {
            score -= 100;
        }
        if (board.at(sq) == Piece::BLACKKNIGHT)
        {
            score -= 300;
        }
        if (board.at(sq) == Piece::BLACKBISHOP)
        {
            score -= 300;
        }
        if (board.at(sq) == Piece::BLACKROOK)
        {
            score -= 500;
        }
        if (board.at(sq) == Piece::BLACKQUEEN)
        {
            score -= 900;
        }
        if (board.at(sq) == Piece::BLACKKING)
        {
            score -= 10000;
        }
    }

    return score;
}