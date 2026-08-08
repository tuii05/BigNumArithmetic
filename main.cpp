#include "BigNum.h"
#include "InputHandler.h"

void fileOption(InputHandler& handler) {
    handler.readFromFile();
}

void consoleOption(InputHandler& handler) {
    handler.readFromConsole();
}

int main() {
    InputHandler handler;
    bool doContinue = true;
    std::string readedLine;
    intType option;
    while(doContinue) {
        std::cout << "1 - Read from a file." << std::endl;
        std::cout << "2 - Console input." << std::endl;
        std::cout << "3 - Exit from the program." << std::endl;
        std::cout << "Please provide which option you would like: ";
        std::getline(std::cin, readedLine);
        option = std::stoi(readedLine);

        switch(option) {
            case 1:
                fileOption(handler);
                break;
            case 2:
                consoleOption(handler);
                break;
            case 3:
                doContinue = false;
                break;
            default:
                std::cout << "There's no such option." << std::endl << std::endl;
                break;
        }
    }
}