#include "movement.h"


#include <iostream>
#include <vector>

bool isWithinBounds(int row, int col){
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

std::vector<Move> getPawnMoves(const Board& board, const Square& start){

    

    std::vector<Move> moves; 

    return moves;
}

std::vector<Move> getKnightMoves(const Board& board, const Square& start){
    Square sq1 = Square(1, 2);
    Square sq2 = Square(3, 3);
    Move mv = Move(sq1, sq2);
    std::vector<Move> moves;
    moves.push_back(mv);
    return moves;
}

std::vector<Move> getBishopMoves(const Board& board, const Square& start){
    Square sq1 = Square(1, 2);
    Square sq2 = Square(3, 3);
    Move mv = Move(sq1, sq2);
    std::vector<Move> moves;
    moves.push_back(mv);
    return moves;
}

std::vector<Move> getRookMoves(const Board& board, const Square& start){
    std::vector<Move> moves;

    // Up
    for (int i = 1; i <= 7; i++){
        if (isWithinBounds(start.row + 1, start.col)){
            Square destination(start.row + 1, start.col);
            Piece piece = board.getPieceAt(destination);
            if (piece == EMPTY){
                // Move myMove(start, destination); ????????
                Square first = Square(start.row, start.col);
                Square second = Square(start.row + 1, start.col);
                Move myMove = Move(first, second);
                moves.push_back(myMove);
            } else {
                Color color = board.getColorAt(destination);
                Color originalColor = board.getColorAt(start);
                if (color != originalColor){
                    Square first = Square(start.row, start.col);
                    Square second = Square(start.row + 1, start.col);
                    Move myMove = Move(first, second);
                    moves.push_back(myMove);
                }
            }
        } else {
            break;
        }
    }

    // Right
    for (int i = 1; i <= 7; i++){
        if (isWithinBounds(start.row, start.col + 1)){
            Square destination(start.row, start.col + 1);
            Piece piece = board.getPieceAt(destination);
            if (piece == EMPTY){
                // Move myMove(start, destination); ????????
                Square first = Square(start.row, start.col);
                Square second = Square(start.row, start.col + 1);
                Move myMove = Move(first, second);
                moves.push_back(myMove);
            } else {
                Color color = board.getColorAt(destination);
                Color originalColor = board.getColorAt(start);
                if (color != originalColor){
                    Square first = Square(start.row, start.col);
                    Square second = Square(start.row, start.col + 1);
                    Move myMove = Move(first, second);
                    moves.push_back(myMove);
                }
            }
        } else {
            break;
        }
    }

    // Left
    for (int i = 1; i <= 7; i++){
        if (isWithinBounds(start.row, start.col - 1)){
            Square destination(start.row, start.col - 1);
            Piece piece = board.getPieceAt(destination);
            if (piece == EMPTY){
                // Move myMove(start, destination); ????????
                Square first = Square(start.row, start.col);
                Square second = Square(start.row, start.col - 1);
                Move myMove = Move(first, second);
                moves.push_back(myMove);
            } else {
                Color color = board.getColorAt(destination);
                Color originalColor = board.getColorAt(start);
                if (color != originalColor){
                    Square first = Square(start.row, start.col);
                    Square second = Square(start.row, start.col - 1);
                    Move myMove = Move(first, second);
                    moves.push_back(myMove);
                }
            }
        } else {
            break;
        }
    }

    // Down
    for (int i = 1; i <= 7; i++){
        if (isWithinBounds(start.row - 1, start.col)){
            Square destination(start.row - 1, start.col);
            Piece piece = board.getPieceAt(destination);
            if (piece == EMPTY){
                // Move myMove(start, destination); ????????
                Square first = Square(start.row, start.col);
                Square second = Square(start.row, start.col - 1);
                Move myMove = Move(first, second);
                moves.push_back(myMove);
            } else {
                Color color = board.getColorAt(destination);
                Color originalColor = board.getColorAt(start);
                if (color != originalColor){
                    Square first = Square(start.row, start.col);
                    Square second = Square(start.row, start.col - 1);
                    Move myMove = Move(first, second);
                    moves.push_back(myMove);
                }
            }
        } else {
        break;
        }
    }

    return moves;
}

std::vector<Move> getQueenMoves(const Board& board, const Square& start){
    Square sq1 = Square(1, 2);
    Square sq2 = Square(3, 3);
    Move mv = Move(sq1, sq2);
    std::vector<Move> moves;
    moves.push_back(mv);
    return moves;
}

std::vector<Move> getKingMoves(const Board& board, const Square& start){
    Square sq1 = Square(1, 2);
    Square sq2 = Square(3, 3);
    Move mv = Move(sq1, sq2);
    std::vector<Move> moves;
    moves.push_back(mv);
    return moves;
}