#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>



enum Piece {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY
};

enum Color {
    WHITE, BLACK, NO_COLOR
};

struct Square {
    int row;
    int col;

    Square(int r, int c) : row(r), col(c) {}

    bool operator==(const Square& other) const{
        return row == other.row && col == other.col;
    }
};

struct Move {
    Square from;
    Square to;
    Piece movedPiece;
    Piece capturedPiece;

    Move(const Square& start, const Square& end) : from(start), to(end), movedPiece(EMPTY), capturedPiece(EMPTY) {}

    Move() : from(Square(0, 0)), to(Square(0, 0)), movedPiece(EMPTY), capturedPiece(EMPTY) {}

    Move(const Square& start, const Square& end, const Piece pieceMoved, const Piece pieceCaptured) : from(start), to(end), movedPiece(pieceMoved), capturedPiece(pieceCaptured) {}

    bool operator==(const Move& other) const{
        return from == other.from && to == other.to && movedPiece == other.movedPiece && capturedPiece == other.capturedPiece;
    }
};

class Board {
    public: 
        Board();

        void initialize();
        void display() const; 

        bool makeMove(const Move& move);
        bool isValidMove(const Move& move) const;
        bool undoMove();

        bool moveInput(const std::string move);

        Piece getPieceAt(const Square& square) const;
        Color getColorAt(const Square& square) const;
        Piece getMovedPiece(const Move& move) const;
        Piece getCapturedPiece(const Move& move) const;

        std::vector<Move> getPawnMoves(const Square& start) const;
        std::vector<Move> getKnightMoves(const Square& start) const;
        std::vector<Move> getBishopMoves(const Square& start) const;
        std::vector<Move> getRookMoves(const Square& start) const;
        std::vector<Move> getQueenMoves(const Square& start) const;
        std::vector<Move> getKingMoves(const Square& start) const;

        std::vector<Move> getMoves(const Square& start) const;
        std::vector<Move> getPlayerMoves(const Color color) const;

        Move getRandomMoveForPlayer(const Color color) const;
        Move getLastMove() const;



    private:
        Piece board[8][8];
        Color colors[8][8];
        Color nextToMove;
        std::vector<Move> moveList;
        bool playAgainstEngine;
};

namespace Helpers {
    void piecePrinter(Piece piece);
}



#endif