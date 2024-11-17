#include "move_gen.h"
#include "board.h"
#include "evaluation.h"

#include <vector>
#include <limits>

Move generateMove(const Board& board){
    Color color = board.getNextToMove();
    std::vector<Move> moves = board.getPlayerMoves(color);
    if (moves.empty()){
        throw std::runtime_error("no moves available");
    }
    Board boardCopy = board;
    Move bestMove = moves[0];
    int eval = (color == WHITE) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    for (size_t i = 0; i < moves.size(); i++){
        Move move = moves[i];
        boardCopy.makeMove(move);
        int newEval = getEvaluation(boardCopy); 
        boardCopy.undoMove();
        if (color == WHITE && newEval > eval || color == BLACK && newEval < eval){
            eval = newEval;
            bestMove = move;
        }
    }
    return bestMove;
}

