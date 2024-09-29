#include "board.h"
#include "helpers.h"

#include <iostream>
#include <string>
#include <random> 

Board::Board(){
    initialize();
}

void Board::initialize(){
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            board[row][col] = EMPTY;
            colors[row][col] = NO_COLOR;
        }
    }

    for (int col = 0; col < 8; col++){
        board[1][col] = PAWN;
        colors[1][col] = WHITE;
        board[6][col] = PAWN;
        colors[6][col] = BLACK;
    }

    board[0][1] = KNIGHT;
    colors[0][1] = WHITE;
    board[0][6] = KNIGHT;
    colors[0][6] = WHITE;
    board[7][1] = KNIGHT;
    colors[7][1] = BLACK;
    board[7][6] = KNIGHT;
    colors[7][6] = BLACK;

    board[0][0] = ROOK;
    colors[0][0] = WHITE;
    board[0][7] = ROOK;
    colors[0][7] = WHITE;
    board[7][0] = ROOK;
    colors[7][0] = BLACK;
    board[7][7] = ROOK;
    colors[7][7] = BLACK;

    board[0][2] = BISHOP;
    colors[0][2] = WHITE;
    board[0][5] = BISHOP;
    colors[0][5] = WHITE;
    board[7][2] = BISHOP;
    colors[7][2] = BLACK;
    board[7][5] = BISHOP;
    colors[7][5] = BLACK;

    board[0][3] = QUEEN;
    colors[0][3] = WHITE;
    board[0][4] = KING;
    colors[0][4] = WHITE;

    board[7][3] = QUEEN;
    colors[7][3] = BLACK;
    board[7][4] = KING;
    colors[7][4] = BLACK;

    nextToMove = WHITE;
    playAgainstEngine = false;
}

void Board::display() const{
    std::string outputString = "";
    for (int row = 7; row >= 0; row--){
        std::string rank = "";
        for (int col = 0; col < 8; col++){
            Color color = colors[row][col];
            Piece piece = board[row][col];
            switch(color){
                case WHITE:
                    switch(piece){
                        case PAWN:
                            rank += "P";
                            break; 
                        case KNIGHT:
                            rank += "N";
                            break;
                        case BISHOP:
                            rank += "B";
                            break;
                        case ROOK:
                            rank += "R";
                            break;
                        case QUEEN:
                            rank += "Q";
                            break;
                        case KING:
                            rank += "K";
                            break;
                        default:
                            break;
                    }
                    break;
                case BLACK:
                    switch(piece){
                        case PAWN:
                            rank += "p";
                            break; 
                        case KNIGHT:
                            rank += "n";
                            break;
                        case BISHOP:
                            rank += "b";
                            break;
                        case ROOK:
                            rank += "r";
                            break;
                        case QUEEN:
                            rank += "q";
                            break;
                        case KING:
                            rank += "k";
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    rank += "□";
                    break;
            }
            rank += " ";
        }
        outputString += rank;
        outputString += '\n';
    }
    outputString += "---------------";
    std::cout << outputString << std::endl;
}

bool Board::isValidMove(const Move& move) const{

    Piece piece = getPieceAt(move.from);
    Color color = getColorAt(move.from);

    if (piece == EMPTY || color != nextToMove){
        return false;
    }
    
    if (piece == PAWN){
        std::vector<Move> pawnMoves = getPawnMoves(move.from);
        bool found = false;
        for (size_t i = 0; i < pawnMoves.size(); i++){
            if (pawnMoves[i] == move){
                found = true;
            }
        }
        return found;
    }

    if (piece == KNIGHT){
        std::vector<Move> knightMoves = getKnightMoves(move.from);
        bool found = false;
        for (size_t i = 0; i < knightMoves.size(); i++){
            if (knightMoves[i] == move){
                found = true;
            }
        }
        return found;
    }

    if (piece == BISHOP){
        std::vector<Move> bishopMoves = getBishopMoves(move.from);
        bool found = false;
        for (size_t i = 0; i < bishopMoves.size(); i++){
            if (bishopMoves[i] == move){
                found = true;
            }
        }
        return found;
    }

    if (piece == ROOK){
        std::vector<Move> rookMoves = getRookMoves(move.from);
        bool found = false;
        for (size_t i = 0; i < rookMoves.size(); i++){
            if (rookMoves[i] == move){
                found = true;
            }
        }
        return found;
    }

    if (piece == QUEEN){
        std::vector<Move> queenMoves = getQueenMoves(move.from);
        bool found = false;
        for (size_t i = 0; i < queenMoves.size(); i++){
            if (queenMoves[i] == move){
                found = true;
            }
        }
        return found;
    }

    if (piece == KING){
        std::vector<Move> kingMoves = getKingMoves(move.from);
        bool found = false;
        for (size_t i = 0; i < kingMoves.size(); i++){
            if (kingMoves[i] == move){
                found = true;
            }
        }
        return found;
    }



    return true;
}

bool Board::makeMove(const Move& move){

    if (!isValidMove(move)){
        return false;
    }

    Square start = move.from;
    Square end = move.to;
    Piece piece = getPieceAt(start);
    Color color = getColorAt(start);
    Piece captured = getCapturedPiece(move);

    if (piece == PAWN && start.row == 6 && color == WHITE){
        char pieceToPromoteTo;
        std::cin >> pieceToPromoteTo;
        switch (pieceToPromoteTo){
            case 'q':
                piece = QUEEN;
                break;
            case 'r':
                piece = ROOK;
                break;
            case 'n':
                piece = KNIGHT;
                break;
            case 'b':
                piece = BISHOP;
                break;
            default:
                std::cerr << "WTF" << std::endl;
                break;
        }
    }

    if (piece == PAWN && start.row == 1 && color == BLACK){
        char pieceToPromoteTo;
        std::cin >> pieceToPromoteTo;
        switch (pieceToPromoteTo){
            case 'q':
                piece = QUEEN;
                break;
            case 'r':
                piece = ROOK;
                break;
            case 'n':
                piece = KNIGHT;
                break;
            case 'b':
                piece = BISHOP;
                break;
            default:
                std::cerr << "WTF" << std::endl;
                break;
        }
    }
    
    if (piece == PAWN && start.row == 4 && color == WHITE){
        if (start.col >= 1 && end.col == start.col - 1){
            if (getLastMove() == Move(Square(start.row + 2, start.col - 1), Square(start.row, start.col - 1), PAWN, EMPTY)){
                board[start.row][end.col] = EMPTY;
                colors[start.row][end.col] = NO_COLOR;
            }
        }
        if (start.col <= 6 && end.col == start.col + 1){
            if (getLastMove() == Move(Square(start.row + 2, start.col + 1), Square(start.row, start.col + 1), PAWN, EMPTY)){
                board[start.row][end.col] = EMPTY;
                colors[start.row][end.col] = NO_COLOR;
            }
        }
    }
    if (piece == PAWN && start.row == 3 && color == BLACK){
        if (start.col >= 1 && end.col == start.col - 1){
            if (getLastMove() == Move(Square(start.row - 2, start.col - 1), Square(start.row, start.col - 1), PAWN, EMPTY)){
                board[start.row][end.col] = EMPTY;
                colors[start.row][end.col] = NO_COLOR;
            }
        }
        if (start.col <= 6 && end.col == start.col + 1){
            if (getLastMove() == Move(Square(start.row - 2, start.col + 1), Square(start.row, start.col + 1), PAWN, EMPTY)){
                board[start.row][end.col] = EMPTY;
                colors[start.row][end.col] = NO_COLOR;
            }
        }
    }

    int startRow = start.row;
    int startCol = start.col;
    int endRow = end.row;
    int endCol = end.col;

    board[startRow][startCol] = EMPTY;
    colors[startRow][startCol] = NO_COLOR;

    board[endRow][endCol] = piece;
    colors[endRow][endCol] = color;

    moveList.push_back(move);

    if (isInCheck(color)){
        undoMove();
    }

    if (nextToMove == BLACK){
        nextToMove = WHITE;
    } else {
        nextToMove = BLACK;
    }
    
    return true;
}

bool Board::moveInput(const std::string move){

    if (move == "stop"){
        std::cout << "Stopping program..." << std::endl;
        return false;
    }

    if (move == "state"){
        if (!hasLegalMoves(nextToMove) && isInCheck(nextToMove)){
            std::cout << "Checkmate" << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }
        return true;
    }


    if (move == "legal"){
        if (hasLegalMoves(nextToMove)){
            std::cout << "Legal move" << std::endl;
        } else {
            std::cout << "No legal moves" << std::endl;
        }
        return true;
    }

    if (move == "player"){
        switch (nextToMove){
            case WHITE:
                std::cout << "White's turn" << std::endl;
                break;
            case BLACK:
                std::cout << "Black's turn" << std::endl;
                break;
            default:
                std::cout << "wtf" << std::endl;
                break;
        }
        return true;
    }

    if (move == "display"){
        display();
        return true;
    }

    if (move == "last"){
        Move lastMove = getLastMove();
        std::cout << Printers::moveToString(lastMove) << std::endl;
        return true;
    }

    if (move == "solo"){
        playAgainstEngine = true;
        std::cout << "Playing against engine" << std::endl;
        return true;
    }

    if (move == "undo"){
        std::cout << "Undoing move" << std::endl;
        undoMove();
        display();
        return true;
    }

    if (move.length() == 2){
        int startCol = move[0] - 'a';
        int startRow = move[1] - '1';
        Square start = Square(startRow, startCol);

        std::vector<Move> moves = getMoves(start);

        for (size_t i = 0; i < moves.size(); i++){
            std::cout << moves[i].to.row << moves[i].to.col << std::endl;
        }

        return true;
    }

    if (move == "white"){
        std::vector<Move> moves = getPlayerMoves(WHITE);
        for (size_t i = 0; i < moves.size(); i++){
            std::cout << moves[i].to.row << moves[i].to.col << std::endl;
        }

        return true;
    }

    if (move == "black"){
        std::vector<Move> moves = getPlayerMoves(BLACK);
        for (size_t i = 0; i < moves.size(); i++){
            std::cout << moves[i].to.row << moves[i].to.col << std::endl;
        }

        return true;
    }

    if (move.length() != 4){
        std::cout << "Input length must be equal to 4 or 2. " << std::endl;
        return true;
    }

    int startCol = move[0] - 'a';
    int startRow = move[1] - '1';
    int endCol = move[2] - 'a';
    int endRow = move[3] - '1';

    Square start = Square(startRow, startCol);
    Square end = Square(endRow, endCol);    
    Move myMove = Move(start, end, getPieceAt(start), getPieceAt(end));

    bool moveSuccess = makeMove(myMove);

    if (!moveSuccess){
        std::cout << "Move failed" << std::endl;
        return true;
    }

    if (moveSuccess && playAgainstEngine){
        Move engineMove = getRandomMoveForPlayer(BLACK);
        std::cout << "Engine move: " << Printers::moveToString(engineMove) << std::endl;
        makeMove(engineMove);
    }

    if (!hasLegalMoves(nextToMove) && isInCheck(nextToMove)){
        std::cout << "CHECKMATE" << std::endl;
        return false;
    }

    display();

    return true;

}

bool Board::undoMove(){
    if (moveList.empty()){
        std::cout << "Nothing to undo" << std::endl;
        return true;
    }

    Move lastMove = moveList.back();

    Square start = lastMove.from;
    Square end = lastMove.to;
    Piece movedPiece = getMovedPiece(lastMove);
    Piece capturedPiece = getCapturedPiece(lastMove);

    int numberOfMoves = moveList.size();
    
    int startRow = start.row;
    int startCol = start.col; 
    int endRow = end.row; 
    int endCol = end.col; 

    board[start.row][start.col] = movedPiece;
    board[end.row][end.col] = capturedPiece;


    if (numberOfMoves % 2 == 1){
        colors[start.row][start.col] = WHITE;
        if (capturedPiece == EMPTY){
            colors[end.row][end.col] = NO_COLOR;
        } else {
            colors[end.row][end.col] = BLACK;
        }
    } else {
        colors[start.row][start.col] = BLACK;
        if (capturedPiece == EMPTY){
            colors[end.row][end.col] = NO_COLOR;
        } else { 
            colors[end.row][end.col] = WHITE;
        }
    }

    moveList.erase(moveList.begin() + moveList.size() - 1);

    if (nextToMove == BLACK){
        nextToMove = WHITE;
    } else {
        nextToMove = BLACK;
    }

    return true;
}

Piece Board::getPieceAt(const Square& square) const{
    return board[square.row][square.col]; 
}

Color Board::getColorAt(const Square& square) const{
    return colors[square.row][square.col];
}

Piece Board::getMovedPiece(const Move& move) const{
    return move.movedPiece;
}

Piece Board::getCapturedPiece(const Move& move) const{
    return move.capturedPiece;
}

std::vector<Move> Board::getPawnMoves(const Square& square) const{
    Piece piece = getPieceAt(square);
    Color color = getColorAt(square);

    int row = square.row;

    std::vector<Move> moves;

    switch (color){
        case WHITE:
            if (row == 1){
                Square destination = Square(row + 1, square.col);
                if (getPieceAt(destination) == EMPTY){
                    moves.push_back(Move(square, destination, PAWN, EMPTY));
                    Square twoStep = Square(row + 2, square.col);
                    if (getPieceAt(twoStep) == EMPTY){
                        moves.push_back(Move(square, twoStep, PAWN, EMPTY));
                    }
                }
                Square takesLeft = Square(row + 1, square.col - 1);
                if (getColorAt(takesLeft) == BLACK && square.col >= 1){
                    moves.push_back(Move(square, takesLeft, PAWN, getPieceAt(takesLeft)));
                }
                Square takesRight = Square(row + 1, square.col + 1);
                if (getColorAt(takesRight) == BLACK && square.col <= 6){
                    moves.push_back(Move(square, takesRight, PAWN, getPieceAt(takesRight)));
                }
            }
            if (row >= 2 && row <= 5){
                Square destination = Square(row + 1, square.col);
                if (getPieceAt(destination) == EMPTY){
                    moves.push_back(Move(square, destination, PAWN, EMPTY));
                }
                Square takesLeft = Square(row + 1, square.col - 1);
                if (getColorAt(takesLeft) == BLACK && square.col >= 1){
                    moves.push_back(Move(square, takesLeft, PAWN, getPieceAt(takesLeft)));
                }
                Square takesRight = Square(row + 1, square.col + 1);
                if (getColorAt(takesRight) == BLACK && square.col <= 6){
                    moves.push_back(Move(square, takesRight, PAWN, getPieceAt(takesRight)));
                }
            }
            if (row == 6){
                Square destination = Square(row + 1, square.col);
                if (getPieceAt(destination) == EMPTY){
                    moves.push_back(Move(square, destination, PAWN, EMPTY));
                }
                Square takesLeft = Square(row + 1, square.col - 1);
                if (getColorAt(takesLeft) == BLACK && square.col >= 1){
                    moves.push_back(Move(square, takesLeft, PAWN, getPieceAt(takesLeft)));
                }
                Square takesRight = Square(row + 1, square.col + 1);
                if (getColorAt(takesRight) == BLACK && square.col <= 6){
                    moves.push_back(Move(square, takesRight, PAWN, getPieceAt(takesRight)));
                }
            }
            if (row == 4){
                if (square.col >= 1){
                    Square takesLeft = Square(row + 1, square.col - 1);
                    if (getPieceAt(takesLeft) == EMPTY && getLastMove() == Move(Square(row + 2, square.col - 1), Square(row, square.col - 1), PAWN, EMPTY)){ // checking if last move was a pawn moving two square forwards
                        moves.push_back(Move(square, takesLeft, PAWN, EMPTY));
                    }
                }
                if (square.col <= 6){
                    Square takesRight = Square(row + 1, square.col + 1);
                    if (getPieceAt(takesRight) == EMPTY && getLastMove() == Move(Square(row + 2, square.col + 1), Square(row, square.col + 1), PAWN, EMPTY)){ // checking if last move was a pawn moving two square forwards
                        moves.push_back(Move(square, takesRight, PAWN, EMPTY));
                    }
                }
            }
            break;
        case BLACK:
            if (row == 6){
                Square destination = Square(row - 1, square.col);
                if (getPieceAt(destination) == EMPTY){
                    moves.push_back(Move(square, destination, PAWN, EMPTY));
                    Square twoStep = Square(row - 2, square.col);
                    if (getPieceAt(twoStep) == EMPTY){
                        moves.push_back(Move(square, twoStep, PAWN, EMPTY));
                    }
                }
                Square takesLeft = Square(row - 1, square.col + 1);
                if (getColorAt(takesLeft) == WHITE && square.col <= 6){
                    moves.push_back(Move(square, takesLeft, PAWN, getPieceAt(takesLeft)));
                }
                Square takesRight = Square(row - 1, square.col - 1);
                if (getColorAt(takesRight) == WHITE && square.col >= 1){
                    moves.push_back(Move(square, takesRight, PAWN, getPieceAt(takesRight)));
                }
            }
            if (row >= 2 && row <= 5){
                Square destination = Square(row - 1, square.col);
                if (getPieceAt(destination) == EMPTY){
                    moves.push_back(Move(square, destination, PAWN, EMPTY));
                }
                Square takesLeft = Square(row - 1, square.col + 1);
                if (getColorAt(takesLeft) == WHITE && square.col <= 6){
                    moves.push_back(Move(square, takesLeft, PAWN, getPieceAt(takesLeft)));
                }
                Square takesRight = Square(row - 1, square.col - 1);
                if (getColorAt(takesRight) == WHITE && square.col >= 1){
                    moves.push_back(Move(square, takesRight, PAWN, getPieceAt(takesRight)));
                }
            }
            if (row == 1){
                Square destination = Square(row - 1, square.col);
                if (getPieceAt(destination) == EMPTY){
                    moves.push_back(Move(square, destination, PAWN, EMPTY));
                }
                Square takesLeft = Square(row - 1, square.col + 1);
                if (getColorAt(takesLeft) == WHITE && square.col <= 6){
                    moves.push_back(Move(square, takesLeft, PAWN, getPieceAt(takesLeft)));
                }
                Square takesRight = Square(row - 1, square.col - 1);
                if (getColorAt(takesRight) == WHITE && square.col >= 1){
                    moves.push_back(Move(square, takesRight, PAWN, getPieceAt(takesRight)));
                }
            }
            if (row == 3){
                if (square.col >= 1){
                    Square takesLeft = Square(row - 1, square.col - 1);
                    if (getPieceAt(takesLeft) == EMPTY && getLastMove() == Move(Square(row - 2, square.col - 1), Square(row, square.col - 1), PAWN, EMPTY)){ // checking if last move was a pawn moving two square forwards
                        moves.push_back(Move(square, takesLeft, PAWN, EMPTY));
                    }
                }
                if (square.col <= 6){
                    Square takesRight = Square(row - 1, square.col + 1);
                    if (getPieceAt(takesRight) == EMPTY && getLastMove() == Move(Square(row - 2, square.col + 1), Square(row, square.col + 1), PAWN, EMPTY)){ // checking if last move was a pawn moving two square forwards
                        moves.push_back(Move(square, takesRight, PAWN, EMPTY));
                    }
                }
            }
            break;
        default:
            break;
    }
    return moves;
}

std::vector<Move> Board::getBishopMoves(const Square& square) const{
    Piece piece = getPieceAt(square);
    Color color = getColorAt(square);
    std::vector<Move> moves;

    // up right
    for (int i = 1; i <= 7; i++){
        int row = square.row + i;
        int col = square.col + i;
        if (row > 7 || col > 7){
            break;
        }
        Square destination = Square(row, col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }

    // up left
    for (int i = 1; i <= 7; i++){
        int row = square.row + i;
        int col = square.col - i;
        if (row > 7 || col < 0){
            break;
        }
        Square destination = Square(row, col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }

    // down right
    for (int i = 1; i <= 7; i++){
        int row = square.row - i;
        int col = square.col + i;
        if (row < 0 || col > 7){
            break;
        }
        Square destination = Square(row, col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }
    
    // down left
    for (int i = 1; i <= 7; i++){
        int row = square.row - i;
        int col = square.col - i;
        if (row < 0 || col < 0){
            break;
        }
        Square destination = Square(row, col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }

    return moves;
}

std::vector<Move> Board::getKnightMoves(const Square& square) const{
    Piece piece = getPieceAt(square);
    Color color = getColorAt(square);
    std::vector<Move> moves;

    int row = square.row;
    int col = square.col; 

    int newRow;
    int newCol;
    
    newRow = row + 2;
    newCol = col + 1;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    newCol = col - 1;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    newRow = row + 1;
    newCol = col + 2;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    newCol = col - 2;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    newRow = row - 1;
    newCol = col + 2;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    newCol = col - 2;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    newRow = row - 2;
    newCol = col + 1;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    newCol = col - 1;
    if (newRow <= 7 && newRow >= 0 && newCol <= 7 && newCol >= 0){
        Square destination = Square(newRow, newCol);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    return moves;
}

std::vector<Move> Board::getRookMoves(const Square& square) const{
    Piece piece = getPieceAt(square);
    Color color = getColorAt(square);
    std::vector<Move> moves;

    // up
    for (int i = 1; i <= 7; i++){
        int row = square.row + i;
        if (row > 7){
            break;
        }
        Square destination = Square(row, square.col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }

    // down
    for (int i = 1; i <= 7; i++){
        int row = square.row - i;
        if (row < 0){
            break;
        }
        Square destination = Square(row, square.col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }

    // left 
    for (int i = 1; i <= 7; i++){
        int col = square.col - i;
        if (col < 0){
            break;
        }
        Square destination = Square(square.row, col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }

    // right 
    for (int i = 1; i <= 7; i++){
        int col = square.col + i;
        if (col > 7){
            break;
        }
        Square destination = Square(square.row, col);
        if (getPieceAt(destination) == EMPTY){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            continue;
        }
        if (getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
            break;
        }
        break;
    }
    
    return moves;
}

std::vector<Move> Board::getQueenMoves(const Square& square) const{
    std::vector<Move> rookMoves = getRookMoves(square);
    std::vector<Move> bishopMoves = getBishopMoves(square);
    std::vector<Move> queenMoves;
    queenMoves.insert(queenMoves.end(), rookMoves.begin(), rookMoves.end());
    queenMoves.insert(queenMoves.end(), bishopMoves.begin(), bishopMoves.end());

    return queenMoves;
}

std::vector<Move> Board::getKingMoves(const Square& square) const{
    Piece piece = getPieceAt(square);
    Color color = getColorAt(square);
    std::vector<Move> moves;

    int row = square.row;
    int col = square.col;

    // up left
    if (row <= 6 && col >= 1){
        Square destination = Square(row + 1, col - 1);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    // up
    if (row <= 6){
        Square destination = Square(row + 1, col);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    // up right
    if (row <= 6 && col <= 6){
        Square destination = Square(row + 1, col + 1);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    // left
    if (col >= 1){
        Square destination = Square(row, col - 1);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    // right
    if (col <= 6){
        Square destination = Square(row, col + 1);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    // down left
    if (row >= 1 && col >= 1){
        Square destination = Square(row - 1, col - 1);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    // down
    if (row >= 1){
        Square destination = Square(row - 1, col);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    // down right
    if (row >= 1 && col <= 6){
        Square destination = Square(row - 1, col + 1);
        if (getPieceAt(destination) == EMPTY || getColorAt(destination) != color){
            moves.push_back(Move(square, destination, getPieceAt(square), getPieceAt(destination)));
        }
    }

    return moves;
}

std::vector<Move> Board::getMoves(const Square& square) const{
    Piece piece = getPieceAt(square);
    std::vector<Move> emptyVector;
    switch(piece){
        case PAWN:
            return getPawnMoves(square);
        case BISHOP:
            return getBishopMoves(square);
        case KNIGHT:
            return getKnightMoves(square);
        case ROOK:
            return getRookMoves(square);
        case QUEEN:
            return getQueenMoves(square);
        case KING:
            return getKingMoves(square);
        default:
            return emptyVector;
    }
}

std::vector<Move> Board::getPlayerMoves(const Color color) const{
    std::vector<Move> moves; 

    for (size_t i = 0; i <= 7; i++){
        for (size_t j = 0; j <= 7; j++){
            Square square = Square(i, j);
            if (getColorAt(square) != color){
                continue;
            }
            std::vector<Move> newMoves = getMoves(square);
            moves.insert(moves.end(), newMoves.begin(), newMoves.end());
        }
    }

    return moves;
}

Move Board::getRandomMoveForPlayer(const Color color) const{
    std::vector<Move> moves = getPlayerMoves(color); 
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, moves.size() - 1);
    return moves[distr(gen)];
}

Move Board::getLastMove() const{
    return moveList[moveList.size() - 1];
}

bool Board::isInCheck(const Color color) const{
    std::vector<Move> moves;

    if (color == BLACK){
        moves = getPlayerMoves(WHITE);
        for (size_t i = 0; i < moves.size(); i++){
            if (moves[i].capturedPiece == KING){
                return true;
            }
        }    
    }

    if (color == WHITE){
        moves = getPlayerMoves(BLACK);
        for (size_t i = 0; i < moves.size(); i++){
            if (moves[i].capturedPiece == KING){
                return true;
            }
        }    
    }

    return false;
} 

bool Board::hasLegalMoves(const Color color){
    std::vector<Move> moves = getPlayerMoves(color);
    bool hasFoundLegalMove = false;
    for (size_t i = 0; i < moves.size(); i++){
        makeMove(moves[i]);
        bool playerIsInCheck = isInCheck(color);
        if (!playerIsInCheck){
            undoMove();
            hasFoundLegalMove = true;
            break;
        }
    }
    return hasFoundLegalMove;
}

