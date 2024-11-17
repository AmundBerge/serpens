#include "board.h"
#include "helpers.h"
#include "evaluation.h"
#include "move_gen.h"

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
    whiteCanCastleShort = true;
    whiteCanCastleLong = true;
    blackCanCastleShort = true;
    blackCanCastleLong = true;

    WHITE_SHORT_CASTLE = Move(Square(0, 1), Square(0, 1), EMPTY, EMPTY);
    WHITE_LONG_CASTLE = Move(Square(0, 2), Square(0, 2), EMPTY, EMPTY);
    BLACK_SHORT_CASTLE = Move(Square(0, 3), Square(0, 3), EMPTY, EMPTY);
    BLACK_LONG_CASTLE = Move(Square(0, 4), Square(0, 4), EMPTY, EMPTY);
    playingInTerminal = false;
    halfMoveCounter = 0;
}

void Board::display() const{
    if (!playingInTerminal){
        std::string s = toString();
        std::cout << s << std::endl;
        return;
    }
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

    if (start == Square(0, 0) && piece == ROOK || end == Square(0, 0) && captured == ROOK){
        whiteCanCastleLong = false;
    }

    if (start == Square(0, 7) || end == Square(0, 7)){
        whiteCanCastleShort = false;
    }

    if (start == Square(7, 0) || end == Square(7, 0)){
        blackCanCastleLong = false;
    }

    if (start == Square(7, 7) || end == Square(7, 7)){
        blackCanCastleShort = false;
    }

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

    if (piece == PAWN || captured != EMPTY){
        halfMoveCounter = 0;
    } else {
        halfMoveCounter += 1; 
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

    if (move == "terminal"){
        playingInTerminal = true;
        return true;
    }
    
    if (move == "eval"){
        std::cout << getEvaluation(*this) << std::endl;
        return true;
    }

    if (move == "movegen"){
        std::cout << Printers::moveToString(generateMove(*this));
        return true;
    }

    if (move == "state"){
        if (!hasLegalMoves(nextToMove) && isInCheck(nextToMove)){
            std::cout << "Checkmate" << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }
        return true;
    }

    if (move == "fifty"){
        if (halfMoveCounter >= 100){
            char wantsToDraw;
            std::cout << "Do you want to draw? (Y/N)" << std::endl;
            std::cin >> wantsToDraw;
            if (wantsToDraw == 'Y'){
                std::cout << "Draw! Stopping program..." << std::endl;
                return false;
            } 
            if (wantsToDraw == 'N'){
                std::cout << "Continuing!" << std::endl;
                return true;
            }
            std::cerr << "what!?" << std::endl;
            return true;
        } else {
            std::cout << "Draw not available" << std::endl;
            return true;
        }
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

    if (move == "short" || move == "long"){
        bool castleSuccess = castle(move);
        if (!castleSuccess){
            std::cout << "Move failed" << std::endl;
        } else {
            display();
        }
        return true;
    }

    if (move == "castle"){
        std::string s = "Castling rights: ";
        if (whiteCanCastleShort){
            s += " | white short";
        }
        if (whiteCanCastleLong){
            s += " | white long";
        }
        if (blackCanCastleShort){
            s += " | black short";
        }
        if (blackCanCastleLong){
            s += " | black long";
        }
        std::cout << s << std::endl;
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

    bool whiteShort = whiteCanCastleShort;
    bool whiteLong = whiteCanCastleLong;
    bool blackShort = blackCanCastleShort;
    bool blackLong = blackCanCastleLong;

    bool moveSuccess = makeMove(myMove);

    if (!moveSuccess){
        std::cout << "Move failed" << std::endl;
        whiteCanCastleShort = whiteShort;
        whiteCanCastleLong = whiteLong;
        blackCanCastleShort = blackShort;
        blackCanCastleLong = blackLong;
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

    if (!hasLegalMoves(nextToMove) && !isInCheck(nextToMove)){
        std::cout << "STALEMATE" << std::endl;
        return false;
    }

    display();

    return true;

}

bool Board::castle(const std::string move){
    Color color = nextToMove;
    if (isInCheck(color)){
        return false;
    }
    std::vector<Square> squaresToCheck; 
    std::vector<Move> opposingPlayerMoves; 
    if (color == WHITE){

        if (move == "short"){
            if (!whiteCanCastleShort){
                return false;
            }
            Square sq1 = Square(0, 4);
            Square sq2 = Square(0, 5);
            Square sq3 = Square(0, 6);
            Square sq4 = Square(0, 7);
            squaresToCheck.push_back(sq1);
            squaresToCheck.push_back(sq2);
            squaresToCheck.push_back(sq3);
            squaresToCheck.push_back(sq4);
        } else if (move == "long"){
            if (!whiteCanCastleLong){
                return false;
            }
            Square sq1 = Square(0, 4);
            Square sq2 = Square(0, 3);
            Square sq3 = Square(0, 2);
            Square sq4 = Square(0, 0);
            squaresToCheck.push_back(sq1);
            squaresToCheck.push_back(sq2);
            squaresToCheck.push_back(sq3);
            squaresToCheck.push_back(sq4);
        } else {
            std::cerr << "Castling error" << std::endl;
        }
    }
    if (color == BLACK){

        if (move == "short"){
            if (!blackCanCastleShort){
                return false;
            }
            Square sq1 = Square(7, 4);
            Square sq2 = Square(7, 5);
            Square sq3 = Square(7, 6);
            Square sq4 = Square(7, 7);
            squaresToCheck.push_back(sq1);
            squaresToCheck.push_back(sq2);
            squaresToCheck.push_back(sq3);
            squaresToCheck.push_back(sq4);
        } else if (move == "long"){
            if (!blackCanCastleLong){
                return false;
            }
            Square sq1 = Square(7, 4);
            Square sq2 = Square(7, 3);
            Square sq3 = Square(7, 2);
            Square sq4 = Square(7, 0);
            squaresToCheck.push_back(sq1);
            squaresToCheck.push_back(sq2);
            squaresToCheck.push_back(sq3);
            squaresToCheck.push_back(sq4);
        } else {
            std::cerr << "Castling error" << std::endl;
        }
    }
    if (!(getPieceAt(squaresToCheck[0]) == KING && getPieceAt(squaresToCheck[1]) == EMPTY && getPieceAt(squaresToCheck[2]) == EMPTY)){
        return false;
    }
    for (size_t i = 0; i < opposingPlayerMoves.size(); i++){
        Square target = opposingPlayerMoves[i].to;
        if (target == squaresToCheck[0] || target == squaresToCheck[1] || target == squaresToCheck[2]){
            return false;
        }
    }
    board[squaresToCheck[0].row][squaresToCheck[0].col] = EMPTY;
    board[squaresToCheck[2].row][squaresToCheck[2].col] = KING;
    board[squaresToCheck[1].row][squaresToCheck[1].col] = ROOK;
    board[squaresToCheck[3].row][squaresToCheck[3].col] = EMPTY;


    colors[squaresToCheck[0].row][squaresToCheck[0].col] = NO_COLOR;
    colors[squaresToCheck[2].row][squaresToCheck[2].col] = color;
    colors[squaresToCheck[1].row][squaresToCheck[1].col] = color;
    colors[squaresToCheck[3].row][squaresToCheck[3].col] = NO_COLOR;

    if (color == WHITE && move == "short"){
        moveList.push_back(WHITE_SHORT_CASTLE);
        whiteCanCastleShort = false;
    } else if (color == WHITE && move == "long"){
        moveList.push_back(WHITE_LONG_CASTLE);
        whiteCanCastleLong = false;
    } else if (color == BLACK && move == "short"){
        moveList.push_back(BLACK_SHORT_CASTLE);
        blackCanCastleShort = false;
    } else if (color == BLACK && move == "long"){
        moveList.push_back(BLACK_LONG_CASTLE);
        blackCanCastleLong = false;
    } else {
        std::cerr << "nah m8" << std::endl;
    }

    if (nextToMove == WHITE){
        nextToMove = BLACK;
    } else {
        nextToMove = WHITE;
    }

    return true;
}

bool Board::undoMove(){
    if (moveList.empty()){
        std::cout << "Nothing to undo" << std::endl;
        return true;
    }

    Move lastMove = moveList.back();

    if (lastMove == WHITE_SHORT_CASTLE){
        board[0][4] = KING;
        board[0][5] = EMPTY;
        board[0][6] = EMPTY;
        board[0][7] = ROOK;
        colors[0][4] = WHITE;
        colors[0][5] = NO_COLOR;
        colors[0][6] = NO_COLOR;
        colors[0][7] = WHITE;
        flipPlayerTurn();
        moveList.erase(moveList.begin() + moveList.size() - 1);
        whiteCanCastleShort = true;
        return true;
    }

    if (lastMove == WHITE_LONG_CASTLE){
        board[0][4] = KING;
        board[0][3] = EMPTY;
        board[0][2] = EMPTY;
        board[0][0] = ROOK;
        colors[0][4] = WHITE;
        colors[0][3] = NO_COLOR;
        colors[0][2] = NO_COLOR;
        colors[0][0] = WHITE;
        flipPlayerTurn();
        moveList.erase(moveList.begin() + moveList.size() - 1);
        whiteCanCastleLong = true;
        return true;
    }

    if (lastMove == BLACK_SHORT_CASTLE){
        board[7][4] = KING;
        board[7][5] = EMPTY;
        board[7][6] = EMPTY;
        board[7][7] = ROOK;
        colors[7][4] = BLACK;
        colors[7][5] = NO_COLOR;
        colors[7][6] = NO_COLOR;
        colors[7][7] = BLACK;
        flipPlayerTurn();
        moveList.erase(moveList.begin() + moveList.size() - 1);
        blackCanCastleShort = true;
        return true;
    }

    if (lastMove == BLACK_LONG_CASTLE){
        board[7][4] = KING;
        board[7][3] = EMPTY;
        board[7][2] = EMPTY;
        board[7][0] = ROOK;
        colors[7][4] = BLACK;
        colors[7][3] = NO_COLOR;
        colors[7][2] = NO_COLOR;
        colors[7][0] = BLACK;
        flipPlayerTurn();
        moveList.erase(moveList.begin() + moveList.size() - 1);
        blackCanCastleLong = true;
        return true;
    }

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
    if (moveList.empty()){
        throw std::out_of_range("nah m8");
    }
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
        // ignore this horrendous code, even by my standards
        bool whiteShort = whiteCanCastleShort;
        bool whiteLong = whiteCanCastleLong;
        bool blackShort = blackCanCastleShort;
        bool blackLong = blackCanCastleLong;
        makeMove(moves[i]);
        whiteCanCastleShort = whiteShort;
        whiteCanCastleLong = whiteLong;
        blackCanCastleShort = blackShort;
        blackCanCastleLong = blackLong;
        bool playerIsInCheck = isInCheck(color);
        if (!playerIsInCheck){
            undoMove();
            hasFoundLegalMove = true;
            break;
        }
    }
    return hasFoundLegalMove;
}

void Board::flipPlayerTurn(){
    if (nextToMove == WHITE){
        nextToMove = BLACK;
    } else {
        nextToMove = WHITE;
    }

    return;
}

Color Board::getNextToMove() const{
    return nextToMove;
}

std::string Board::toString() const{
    std::string str = "_X_";
    for (size_t i = 0; i < 8; i++){
        for (size_t j = 0; j < 8; j++){
            Piece piece = getPieceAt(Square(i, j));
            Color color = getColorAt(Square(i, j));
            switch (piece){
                case EMPTY: 
                    str += "□";
                    break;
                case PAWN:
                    str += color == WHITE ? "P" : "p";
                    break;
                case KNIGHT:
                    str += color == WHITE ? "N" : "n";
                    break;
                case BISHOP:
                    str += color == WHITE ? "B" : "b";
                    break;
                case ROOK:
                    str += color == WHITE ? "R" : "r";
                    break;
                case QUEEN:
                    str += color == WHITE ? "Q" : "q";
                    break;
                case KING: 
                    str += color == WHITE ? "K" : "k";
                    break;
                default: 
                    std::cerr << "Something went wrong" << std::endl;
                    break;
            }
        }
    }
    str += "-X-";
    str += nextToMove == WHITE ? "W" : "B";
    str += whiteCanCastleShort ? "1" : "0";
    str += whiteCanCastleLong ? "1" : "0";
    str += blackCanCastleShort ? "1" : "0";
    str += blackCanCastleLong ? "1" : "0";
    if (halfMoveCounter < 10){
        str += "0";
    } else if (halfMoveCounter >= 50){
        str += "50";
    } else {
        str += std::to_string(halfMoveCounter);
    }
    // better to use moveToString9 directly, should be made to handle castling
    // Move lastMove = getLastMove();
    Move lastMove = Move(Square(0,0),Square(0,0),EMPTY,EMPTY);
    std::string simpleLastMoveString = Printers::moveToString(lastMove);
    if (simpleLastMoveString == "b1b1" || simpleLastMoveString == "c1c1" || simpleLastMoveString == "d1d1" || simpleLastMoveString == "e1e1"){
        str += "9999";
        str += simpleLastMoveString;
    } else { 
        str += "";
    }





    return str;

    
}

/* 
std::string Board::toString() const{
    std::string str = "_X_";
    for (size_t i = 0; i < 8; i++){
        for (size_t j = 0; j < 8; j++){
            Piece piece = getPieceAt(Square(i, j));
            Color color = getColorAt(Square(i, j));
            switch (piece){
                case EMPTY:
                    str += "□";
                    break;
                case PAWN:
                    str += color == WHITE ? 'P' : 'p';
                    break;
                case KNIGHT:
                    str += color == WHITE ? 'N' : 'n';
                    break;
                case BISHOP:
                    str += color == WHITE ? 'B' : 'b';
                    break;
                case ROOK:
                    str += color == WHITE ? 'R' : 'r';
                    break;
                case QUEEN:
                    str += color == WHITE ? 'Q' : 'q';
                    break;
                case KING:
                    str += color == WHITE ? 'K' : 'k';
                    break;
                default: 
                    std::cerr << "Wuuuuut" << std::endl;
                    break;
            }
        }
    }
    str += "-X-";
    str += nextToMove == WHITE ? "W" : "B";
    str += whiteCanCastleShort ? "1" : "0";
    str += whiteCanCastleLong ? "1" : "0";
    str += blackCanCastleShort ? "1" : "0";
    str += blackCanCastleLong ? "1" : "0";
     std::string halfMoveCounterString = std::to_string(halfMoveCounter);
    if (halfMoveCounter <= 9){
        str += "_";
    }
    if (halfMoveCounter <= 99){
        str += "_";
    }
    str += halfMoveCounterString;
    return str;
} 
*/

