#include "evaluation.hpp"

inline int flip(int sq)
{
    // this flips rank from top to bottom
    return sq ^ 56;
}

// PeSTO's evaluation function
int mg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0,
};

int eg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0,
};

int mg_knight_table[64] = {
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23,
};

int eg_knight_table[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64,
};

int mg_bishop_table[64] = {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21,
};

int eg_bishop_table[64] = {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int mg_rook_table[64] = {
     32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26,
};

int eg_rook_table[64] = {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20,
};

int mg_queen_table[64] = {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50,
};

int eg_queen_table[64] = {
     -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41,
};

int mg_king_table[64] = {
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14,
};

int eg_king_table[64] = {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};

// // We'll group them by piece index p = 0..5, i.e. PAWN..KING
int *mg_pesto_table[6] = {
    mg_pawn_table,
    mg_knight_table,
    mg_bishop_table,
    mg_rook_table,
    mg_queen_table,
    mg_king_table};
int *eg_pesto_table[6] = {
    eg_pawn_table,
    eg_knight_table,
    eg_bishop_table,
    eg_rook_table,
    eg_queen_table,
    eg_king_table};

// -----------------------------------------------------------------------------
// 6) The actual evaluation function (PESTO style).
//    Here, we do:
//       - sum up mid-game (mg) and end-game (eg) scores for both sides
//       - track "gamePhase" based on how many rooks/queens/knights/bishops exist
//       - do a tapered eval:
//            final = (mgScore * mgPhase + egScore * egPhase) / 24
//
//    If your Board has a method like board.sideToMove() returning Color::WHITE or
//    Color::BLACK, map that to (0 or 1). Then do mgScore[ side ] - mgScore[ other(side) ].
// -----------------------------------------------------------------------------
int heuristic(Board &board)
{
    int white_score_mg = 0;
    int white_score_eg = 0;
    int black_score_mg = 0;
    int black_score_eg = 0;
    int phase_score = 0;
    for (Square sq = Square::underlying::SQ_A1; sq <= Square::underlying::SQ_H8; ++sq)
    {
        if (board.at(sq) == Piece::WHITEPAWN)
        {
            white_score_mg += 82 + mg_pesto_table[0][flip(sq.index())];
            white_score_eg += 94 + eg_pesto_table[0][flip(sq.index())];
        }
        if (board.at(sq) == Piece::WHITEKNIGHT)
        {
            white_score_mg += 337 + mg_pesto_table[1][flip(sq.index())];
            white_score_eg += 281 + eg_pesto_table[1][flip(sq.index())];
            phase_score += 1;
        }
        if (board.at(sq) == Piece::WHITEBISHOP)
        {
            white_score_mg += 365 + mg_pesto_table[2][flip(sq.index())];
            white_score_eg += 297 + eg_pesto_table[2][flip(sq.index())];
            phase_score += 1;
        }
        if (board.at(sq) == Piece::WHITEROOK)
        {
            white_score_mg += 477 + mg_pesto_table[3][flip(sq.index())];
            white_score_eg += 512 + eg_pesto_table[3][flip(sq.index())];
            phase_score += 2;
        }
        if (board.at(sq) == Piece::WHITEQUEEN)
        {
            white_score_mg += 1025 + mg_pesto_table[4][flip(sq.index())];
            white_score_eg += 936 + eg_pesto_table[4][flip(sq.index())];
            phase_score += 4;
        }
        if (board.at(sq) == Piece::WHITEKING)
        {
            white_score_mg += 10000 + mg_pesto_table[5][flip(sq.index())];
            white_score_eg += 10000 + eg_pesto_table[5][flip(sq.index())];
        }
        if (board.at(sq) == Piece::BLACKPAWN)
        {
            black_score_mg += 82 + mg_pesto_table[0][sq.index()];
            black_score_eg += 94 + eg_pesto_table[0][sq.index()];
        }
        if (board.at(sq) == Piece::BLACKKNIGHT)
        {
            black_score_mg += 337 + mg_pesto_table[1][sq.index()];
            black_score_eg += 281 + eg_pesto_table[1][sq.index()];
            phase_score += 1;
        }
        if (board.at(sq) == Piece::BLACKBISHOP)
        {
            black_score_mg += 365 + mg_pesto_table[2][sq.index()];
            black_score_eg += 297 + eg_pesto_table[2][sq.index()];
            phase_score += 1;
        }
        if (board.at(sq) == Piece::BLACKROOK)
        {
            black_score_mg += 477 + mg_pesto_table[3][sq.index()];
            black_score_eg += 512 + eg_pesto_table[3][sq.index()];
            phase_score += 2;
        }
        if (board.at(sq) == Piece::BLACKQUEEN)
        {
            black_score_mg += 1025 + mg_pesto_table[4][sq.index()];
            black_score_eg += 936 + eg_pesto_table[4][sq.index()];
            phase_score += 4;
        }
        if (board.at(sq) == Piece::BLACKKING)
        {
            black_score_mg += 10000 + mg_pesto_table[5][sq.index()];
            black_score_eg += 10000 + eg_pesto_table[5][sq.index()];
        }
    }

    int mg_score = white_score_mg - black_score_mg;
    int eg_score = white_score_eg - black_score_eg;
    return (mg_score * phase_score + eg_score * (24 - phase_score)) / 24;
}
