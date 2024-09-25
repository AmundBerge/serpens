#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <iostream>
#include <vector>

#include "board.h"

std::vector<Move> getPawnMoves(const Board& board, const Square& start);
std::vector<Move> getBishopMoves(const Board& board, const Square& start);
std::vector<Move> getKnightMoves(const Board& board, const Square& start);
std::vector<Move> getRookMoves(const Board& board, const Square& start);
std::vector<Move> getQueenMoves(const Board& board, const Square& start);
std::vector<Move> getKingMoves(const Board& board, const Square& start);


#endif