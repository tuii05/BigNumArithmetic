#pragma once
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include "Definitions.h"

class Storage {
    public:
        Storage() : isPositive(true) {}
        explicit Storage(const std::string &number, bool isPositive);
        explicit Storage(const std::string &number);
        explicit Storage(std::vector<intType>&&, bool);
        Storage(Storage&& moved) {
            this->digits = std::move(moved.digits);
            this->isPositive = moved.isPositive;
        }
        Storage(const Storage &) = delete;

    public:
        Storage& operator=(Storage&& moved) {
            if(this != &moved) {
                this->digits = std::move(moved.digits);
                this->isPositive = moved.isPositive;
            }
            return *this;
        }

        Storage &operator=(const Storage &) = delete;

    public:
        itPair getConvertedDigits() const { return {digits.begin(), digits.end()}; }
        bool isPos() const { return isPositive; }

        void setNumber(std::string&& number, const intType base) {
            if (number.empty())
                throw std::invalid_argument("Empty string");

            std::string num = number;

            if (num[0] == '-')
            {
                this->isPositive = false;
                num.erase(num.begin());
            }
            else if (num[0] == '+')
            {
                this->isPositive = true;
                num.erase(num.begin());
            }
            else if (isdigit(num[0]))
            {
                this->isPositive = true;
            }
            else
            {
                throw std::invalid_argument("The beginning of the string contains a character that is not a number or +,- sign.");
            }

            while (num.size() > 1 && num[0] == '0') {
                num.erase(num.begin());
            }
            convertToBase(std::move(num), base, digits);
        }

        void setNumber(std::vector<intType>&& vect) {
            digits = std::move(vect);
        }

    public:
        void convertFromString(const std::string &number, const intType base);
        std::string convertToString(const intType base) const;
        //std::vector<intType> convertToBase(std::string num, const intType base) const;
        std::vector<intType> convertToBase(const std::string &numStr, intType base) const;
        void convertToBase(std::string &&numStr, intType base, std::vector<intType> &digits) const;

    private:
        std::vector<intType> digits;
        bool isPositive;
};