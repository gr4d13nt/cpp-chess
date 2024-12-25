#include "evaluation.hpp"

int heuristic(Board &board)
{
    int score = 0;
    for (Square sq = Square::underlying::SQ_A1; sq <= Square::underlying::SQ_H8; ++sq) {
        if (board.at(sq) == Piece::WHITEPAWN) {
            score += 100;
        }
        if (board.at(sq) == Piece::WHITEKNIGHT) {
            score += 300;
        }
        if (board.at(sq) == Piece::WHITEBISHOP) {
            score += 300;
        }
        if (board.at(sq) == Piece::WHITEROOK) {
            score += 500;
        }
        if (board.at(sq) == Piece::WHITEQUEEN) {
            score += 900;
        }
        if (board.at(sq) == Piece::WHITEKING) {
            score += 10000;
        }
        if (board.at(sq) == Piece::BLACKPAWN) {
            score -= 100;
        }
        if (board.at(sq) == Piece::BLACKKNIGHT) {
            score -= 300;
        }
        if (board.at(sq) == Piece::BLACKBISHOP) {
            score -= 300;
        }
        if (board.at(sq) == Piece::BLACKROOK) {
            score -= 500;
        }
        if (board.at(sq) == Piece::BLACKQUEEN) {
            score -= 900;
        }
        if (board.at(sq) == Piece::BLACKKING) {
            score -= 10000;
        }
    }

    return score;
}