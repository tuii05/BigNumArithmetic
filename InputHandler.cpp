#include "InputHandler.h"
#include <fstream>

//Function for handling the input correctly.
void InputHandler::handleInput() {
    if (isRPN()) // Checks if the input was written in RPN.
    {
        #if DEBUG
        std::cout << "handleRPN()" << std::endl;
        #endif
        handleRPN();
    }
    else
    {
        handleNormal();
    }
}

//Handles console inputs.
void InputHandler::readFromConsole() {
    input.clear();
    std::cout << "Give an expression to be evaluated: ";
    std::getline(std::cin, input);
    if(input.empty()) {
        throw std::runtime_error("The input is invalid.");
    }
    handleInput();
}

//Handles file reading.
void InputHandler::readFromFile() {
    input.clear();
    std::string fileName;
    std::cout << "Please provide the name of the file: ";
    std::getline(std::cin, fileName);
    std::ifstream file(fileName, std::ios::in | std::ios::binary);
    if(!file.is_open()) {
        throw std::runtime_error("Failed to open the following file: " + fileName);
    }
    input.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    handleInput();
}

bool InputHandler::isValidChar(char character) {
    return std::find(validCharacters.begin(), validCharacters.end(), character) != validCharacters.end();
}

// Checks if the input is valid for the normal case.
bool InputHandler::isValidInput() {
    inputVector.clear();
    size_t inputSize = input.size();
    if (inputSize < 3 || (!isdigit(input[0]) && (input[0] != '(' && input[0] != '-')))
    {
        #if DEBUG
        std::cout << inputSize << std::endl;
        std::cout << isdigit(input[0]) << " " << input[0] << std::endl;
        #endif
        throw std::runtime_error("The input is invalid. - IVI");
    }
    size_t operatorCount = 0, digitCount = 0, i = 0;
    std::string sign;
    bool expectNumber = true;
    while(i < inputSize) {
        if(isspace(input[i])) {
            ++i;
            continue;
        }
        if(isdigit(input[i])) {
            size_t begin = i;
            while(i < inputSize && isdigit(input[i])) {
                ++i;
            }
            std::string number = input.substr(begin, i - begin);
            if(!sign.empty()) {
                number = sign + number;
                sign.clear();
            }
            inputVector.push_back(number);
            ++digitCount;
            expectNumber = false;
            continue;
        }
        if(isValidChar(input[i])) {
            if(input[i] == '+' || input[i] == '-') {
                size_t neg = 0;
                while(i < inputSize && (input[i] == '+' || input[i] == '-')) {
                    if(input[i] == '-') {
                        ++neg;
                    }
                    i++;
                }
                std::string operator_ = (neg % 2 == 0 ? "+" : "-");
                if(expectNumber) {
                    sign = operator_;
                    continue;
                }
                else {
                    inputVector.push_back(operator_);
                    ++operatorCount;
                    expectNumber = true;
                    continue;
                }
            }
            if(input[i] == '*' || input[i] == '/') {
                if(expectNumber) {
                    throw std::runtime_error("Operator" + std::string(1,input[i]) + "is not expected.");
                }
                std::string operator_(1, input[i]);
                inputVector.push_back(operator_);
                ++operatorCount;
                ++i;
                if(i < inputSize && (input[i] == '+' || input[i] == '-')) {
                    size_t neg = 0;
                    while(i < inputSize && (input[i] == '+' || input[i] == '-')) {
                        if(input[i] == '-') {
                            ++neg;
                        }
                        ++i;
                    }
                    sign = (neg % 2 == 0) ? "+" : "-";
                }
                continue;
            }
            if(input[i] == '(' || input[i] == ')') {
                std::string par(1, input[i]);
                inputVector.push_back(par);
                ++i;
                continue;
            }
        }
        throw std::runtime_error("The input contains invalid characters.");
    }
    if(expectNumber) {
        throw std::runtime_error("The input format is wrong.");
    }
    #if DEBUG
    std::cout << "Vector: " << std::endl;
    for(auto v : inputVector) {
        std::cout << v;
    }
    #endif
    std::cout << std::endl;
    return digitCount >= 2 && operatorCount == (digitCount - 1);
}

//Checks if the input is valid for RPN.
bool InputHandler::isValidInputRPN() {
    inputVector.clear();
    size_t i = 0;
    size_t digitCount = 0, operatorCount = 0;
    size_t inputSize = input.size();
    std::string sign, operator_;
    while(i < inputSize) {
        if(isspace(input[i])) {
            ++i;
            continue;
        }

        if(isdigit(input[i])) {
            size_t begin = i;
            while(i < input.size() && isdigit(input[i])) {
                ++i;
            }
            std::string number = input.substr(begin, i - begin);
            if(!sign.empty()) {
                number = sign + number;
                sign.clear();
            }
            inputVector.push_back(number);
            ++digitCount;
            continue;
        }
        if(isValidChar(input[i])) {
            if((input[i] == '+' || input[i] == '-') && i + 1 < inputSize && isdigit(input[i+1])) {
                if(input[i] == '-') {
                    sign = "-";
                }
                ++i;
                continue;
            }
            operator_ = input[i];
            inputVector.push_back(operator_);
            ++i;
            ++operatorCount;
            continue;
        }
        throw std::runtime_error("Invalid character in RPN.");
    }
    #if DEBUG
    std::cout << "Vector: ";
    for(auto &v : inputVector) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    #endif

    return operatorCount == (digitCount - 1);
}

//Check if the input was written in RPN.
bool InputHandler::isRPN() {
    while(!input.empty() && isspace(input.back())) {
        input.pop_back();
    }
    char lastChar = input.back();
    if(isdigit(lastChar)) {
        #if DEBUG
        std::cout << "It is not RPN." << std::endl;
        #endif
        if (!isValidInput())
        {
            throw std::runtime_error("The input is invalid for normal case.");
        }
        return false;
    }
    else if (!isdigit(lastChar) && isValidChar(lastChar)) {
        if(!isValidInputRPN()) {
            throw std::runtime_error("The input is invalid for RPN case.");
        }
        return true;
    }
    else {
        throw std::runtime_error("The input is invalid. - 2");
        return false;
    }
}

void InputHandler::execute(std::string operator_, const BigNum &b, BigNum &result, BigNum &remainderUpper, BigNum &remainderLower) {
    if (operator_ == "+")
    {
        result = result + b;
    }
    else if (operator_ == "-")
    {
        #if DEBUG
        std::cerr << "Operator-" << std::endl;
        #endif
        result = result - b;
    }
    else if (operator_ == "*")
    {
        result = result * b;
    }
    else if (operator_ == "/" || operator_ == ":")
    {
        std::pair<BigNum, BigNum> QR = result / b;
        result = std::move(QR.first);
        remainderUpper = remainderUpper + (QR.second * remainderLower);
        remainderLower = remainderLower * b;
    }
}

void InputHandler::handleRPN() {
    intType base = BASENUM;
    BigNum a, b, remainderUpper("0", base), remainderLower("1", base);
    std::stack<BigNum> stack;
    for(auto &v : inputVector) {
        if(isdigit(v[0]) || ( (v[0] == '+' || v[0] == '-') && v.size() > 1 && isdigit(v[1]) )) {
            BigNum number;
            auto v_ = v;
            number.setNumber(std::move(v_), base);
            stack.push(std::move(number));
        }
        else if (isValidChar(v[0])) {
            if(stack.size() < 2) {
                throw std::runtime_error("Not enough numbers for the operator.");
            }
            b = std::move(stack.top());
            stack.pop();
            a = std::move(stack.top());
            stack.pop();
            execute(v, b, a, remainderUpper, remainderLower);
            stack.push(std::move(a));
        }
        else {
            throw std::runtime_error("Invalid chacter in RPN.");
        }
    }
    BigNum result(std::move(stack.top()));
    //BigNum zero("0", base);
    if(remainderUpper.getDigits().second != remainderUpper.getDigits().first) {
        #if DEBUG
        std::cerr << remainderUpper << "! ";
        std::cerr << remainderLower << "! " << std::endl;
        #endif
        BigNum gcd_;
        gcd(remainderUpper, remainderLower, gcd_);
        remainderUpper = std::move((remainderUpper / gcd_).first);
        remainderLower = std::move((remainderLower / gcd_).first);
        #if DEBUG
        std::cerr << gcd_ << "! ";
        std::cerr << remainderUpper << "! ";
        std::cerr << remainderLower << "! " << std::endl;
        #endif
    }
    std::pair<BigNum, BigNum> resultWithRemainder = ((result * remainderLower) + remainderUpper) / remainderLower;
    std::cout << resultWithRemainder;
}

int InputHandler::getPrecedence(const std::string &operator_) const
{
    if (operator_ == "+" || operator_ == "-")
        return 1;
    if (operator_ == "*" || operator_ == "/" || operator_ == ":")
        return 2;
    return 0;
}

void InputHandler::handleNormal()
{
    std::stack<std::string> operators;
    std::deque<std::string> convertedOutput;

    while (!inputVector.empty())
    {
        std::string current = inputVector.front();
        inputVector.pop_front();

        if (isdigit(current[0]) || (current.size() > 1 && (current[0] == '+' || current[0] == '-') && isdigit(current[1])))
        {
            convertedOutput.push_back(std::move(current));
        }
        else if (current == "(")
        {
            operators.push(std::move(current));
        }
        else if (current == ")")
        {
            while (!operators.empty() && operators.top() != "(")
            {
                convertedOutput.push_back(std::move(operators.top()));
                operators.pop();
            }
            if (operators.empty())
            {
                throw std::runtime_error("Parentheses are mismatched, missing '('.");
            }
            operators.pop();
        }
        else if (isValidChar(current[0]))
        {
            int precedence = getPrecedence(current);

            while (!operators.empty() && operators.top() != "(" && getPrecedence(operators.top()) >= precedence)
            {
                convertedOutput.push_back(std::move(operators.top()));
                operators.pop();
            }
            operators.push(std::move(current));
        }
        else
        {
            throw std::runtime_error("Invalid character found during handling normal case.");
        }
    }

    while (!operators.empty())
    {
        if (operators.top() == "(")
        {
            throw std::runtime_error("Parentheses are mismatched, missing ')'");
        }
        convertedOutput.push_back(std::move(operators.top()));
        operators.pop();
    }

    inputVector = std::move(convertedOutput);

    handleRPN();
}
