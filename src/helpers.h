#ifndef HELPERS_H
#define HELPERS_H

#include "board.h"
#include <string> 

namespace Printers {
    void piecePrinter(Piece piece);
    void squarePrinter(Square square);
    void movePrinter(Move move);

    std::string pieceToString(Piece piece);
    std::string squareToString(Square square);
    std::string moveToString(Move move);
}

#endif