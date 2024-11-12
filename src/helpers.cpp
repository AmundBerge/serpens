#include <iostream>
#include "board.h"

namespace Printers
{
    void piecePrinter(Piece piece)
    {
        switch (piece)
        {
        case EMPTY:
            std::cout << "Empty" << std::endl;
            break;
        case PAWN:
            std::cout << "Pawn" << std::endl;
            break;
        case BISHOP:
            std::cout << "Bishop" << std::endl;
            break;
        case KNIGHT:
            std::cout << "Knight" << std::endl;
            break;
        case ROOK:
            std::cout << "ROOK" << std::endl;
            break;
        case QUEEN:
            std::cout << "QUEEN" << std::endl;
            break;
        case KING:
            std::cout << "KING" << std::endl;
            break;
        default:
            std::cout << "Hmmmmm..." << std::endl;
            break;
        }
    };

    void squarePrinter(Square square)
    {
        int row = square.row;
        int col = square.col;
        char letter = static_cast<char>('a' + col);
        char number = row + 1;
        std::cout << "Square info: " << letter << number << std::endl;
        std::cout << "Row: " << row << " | Col: " << col << std::endl;
    };

    void movePrinter(Move move)
    {
        std::cout << "Move info: " << std::endl;
        Square start = move.from;
        Square end = move.to;
        Piece movedPiece = move.movedPiece;
        Piece capturedPiece = move.capturedPiece;
        squarePrinter(start);
        squarePrinter(end);
        piecePrinter(movedPiece);
        piecePrinter(capturedPiece);
    };

    std::string pieceToString(Piece piece){
        switch (piece){
            case EMPTY: 
                return "Empty";
            case PAWN:
                return "Pawn";
            case KNIGHT: 
                return "Knight";
            case BISHOP:
                return "Bishop";
            case ROOK:
                return "Rook";
            case QUEEN:
                return "Queen";
            case KING:
                return "King";
            default:
                return "Error in finding piece type";
        }
    }

    std::string squareToString(Square square){
        int row = square.row;
        int col = square.col;
        char letter = static_cast<char>('a' + col);
        char number = '0' + (row + 1);
        std::string str = "";
        str += letter;
        str += number; 
        return str;
    }

    std::string moveToString(Move move){
        Square start = move.from;
        Square end = move.to;
        std::string str = "";
        str += squareToString(start);
        str += squareToString(end);
        return str;
    }

    std::string moveToString8(Move move){
        std::string str = "";
        return str;
    }

}