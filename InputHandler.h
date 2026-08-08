#pragma once
#include "BigNum.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <utility>
#include <functional>
#include <stack>

class InputHandler {
    public:
        InputHandler() : validCharacters{'+', '-', '*', '/', ':', '(', ')'} {}
        InputHandler(const InputHandler &) = delete;
        InputHandler(InputHandler &&) = delete;

    public:
        void readFromFile();
        void readFromConsole();

    public:
        bool isRPN();
        bool isValidInput();
        bool isValidInputRPN();
        bool isValidChar(char);

    public:
        int getPrecedence(const std::string &op) const;

    public:
        void execute(std::string, const BigNum&, BigNum&, BigNum&, BigNum&);

    public:
        void handleInput();
        void handleRPN();
        void handleNormal();

    public:
        InputHandler &operator=(const InputHandler &) = delete;
        InputHandler &operator=(InputHandler &&) = delete;

    private:
        std::string input;
        std::deque<std::string> inputVector;
        const std::vector<char> validCharacters;
};
