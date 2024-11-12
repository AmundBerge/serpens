#include "evaluation.h"
#include "board.h"


int getEvaluation(const Board& board){
    int eval = 0;
    for (size_t i = 0; i < 8; i++){
        for (size_t j = 0; j < 8; j++){
            Square square = Square(i, j);
            eval += getSquareMaterial(board, square);
        }
    }
    return eval;
}

int getSquareMaterial(const Board& board, const Square& square){
    Piece piece = board.getPieceAt(square);
    Color color = board.getColorAt(square);
    switch (piece){
        case PAWN: 
            return color == WHITE ? 1 : -1;
        case KNIGHT:
            return color == WHITE ? 3 : -3;
        case BISHOP:
            return color == WHITE ? 3 : -3;
        case ROOK:
            return color == WHITE ? 5 : -5;
        case QUEEN:
            return color == WHITE ? 9 : -9;
        default:
            return 0; // no reason to check king
    }
}
