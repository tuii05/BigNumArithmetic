#include "Storage.h"


//Convert and slicing the string into a vector, so this way we can handle arithmetics.
void Storage::convertFromString(const std::string& number, const intType base) {
    if(number.empty()) {
        throw std::invalid_argument("The string is empty.");
    }

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

    while(num.size() > 1 && num[0] == '0') {
        num.erase(num.begin());
    }
    digits = std::move(convertToBase(num, base));
}

//Convert the vector back to a string.
std::string Storage::convertToString(const intType base) const
{
    if (digits.empty())
        return "0";

    std::string converted;

    if (!isPositive)
        converted.push_back('-');

    const unsigned width = std::to_string(base - 1).size();

    converted += std::to_string(digits.back());

    char buffer[32];

    for (size_t i = digits.size() - 1; i-- > 0;)
    {
        std::snprintf(buffer, sizeof(buffer), "%0*u", width, digits[i]);
        converted += buffer;
    }

    return converted;
}

// Convert the string into a vector containing the appropriate numbers in a certain base. By default the converted base is 1e9.
std::vector<intType> Storage::convertToBase(const std::string &numStr, intType base) const
{
    std::vector<intType> digits;
    std::string current = numStr;

    while (!(current.size() == 1 && current[0] == '0'))
    {
        uint64_t carry = 0;
        std::string next;
        next.reserve(current.size());

        for (char ch : current)
        {
            carry = carry * 10 + (ch - '0');
            intType quotient = carry / base;
            carry %= base;

            if (!next.empty() || quotient != 0)
                next += std::to_string(quotient);
        }

        digits.push_back((intType)carry);

        if (next.empty())
            next = "0";

        current.swap(next);
    }

    return digits;
}


//Convert the string into a vector containing the appropriate numbers in a certain base. By default the converted base is 1e9.
void Storage::convertToBase(std::string &&numStr, intType base, std::vector<intType>& digits) const
{
    digits.clear();
    std::string current = numStr;

    while (!(current.size() == 1 && current[0] == '0'))
    {
        uint64_t carry = 0;
        std::string next;
        next.reserve(current.size());

        for (char ch : current)
        {
            carry = carry * 10 + (ch - '0');
            intType quotient = carry / base;
            carry %= base;

            if (!next.empty() || quotient != 0)
                next += std::to_string(quotient);
        }

        digits.push_back((intType)carry);

        if (next.empty())
            next = "0";

        current.swap(next);
    }
}

Storage::Storage(const std::string &number)
{
    if (number.empty()) {
        throw std::invalid_argument("Empty string");
    }

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

    digits = convertToBase(num, BASENUM);
}

Storage::Storage(const std::string &number, bool isPositive)
{
    this->isPositive = isPositive;
    if (number.empty())
        throw std::invalid_argument("Empty string");

    std::string num = number;

    if (num[0] == '-')
    {
        num.erase(num.begin());
        this->isPositive = false;
    }
    else if (num[0] == '+')
    {
        num.erase(num.begin());
        this->isPositive = true;
    }

    while (num.size() > 1 && num[0] == '0')
        num.erase(num.begin());

    digits = convertToBase(num, BASENUM);
}

Storage::Storage(std::vector<intType> &&digits, bool isPositive) {
    this->digits = std::move(digits);
    this->isPositive = isPositive;
}