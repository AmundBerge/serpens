#include "board.h"



#include <iostream>


int main(){
    Board board;
    board.display();
    std::string input;

    while (true){
        std::cout << "Please enter your move: " << std::endl;
        std::cin >> input;
        bool moveSuccess = board.moveInput(input);
        if (!moveSuccess){
            break;
        }
        
    }

    std::string test = board.toString();
    std::cout << test << std::endl;

    return 0;
}